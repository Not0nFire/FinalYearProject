#include <include/SceneManager.hpp>

std::unique_ptr<SceneManager> SceneManager::mInstance = nullptr;

SceneManager::SceneManager() {
	mRequestThread = std::thread(std::bind(&SceneManager::handleSceneRequests, this));
}

void SceneManager::handleSceneRequests() {
	while (true) {
		if (mStopRequestThread) {
			//Break out of while loop and return. Lock will release upon destruction.
			break;
		}

		std::unique_lock<std::mutex> requestLock(mRequestMutex);
		while (mRequests.empty()) {
			mRequestPending.wait(requestLock);
		}

		//Fetch a request from the queue
		auto request = mRequests.front();

		std::lock_guard<std::mutex> sceneLock(mSceneMutex);

		switch (request.type) {
		case detail::SceneRequest::NAVIGATE:
			processNavigateRequest(request);
			break;

		case detail::SceneRequest::ADD_TO_MANAGER:
			processAddRequest(request);
			break;

		case detail::SceneRequest::BOTH:
			processAddRequest(request);
			processNavigateRequest(request);
			break;

		}//end switch(type)

		mRequests.pop();
	}//end while true
}

void SceneManager::processAddRequest(detail::SceneRequest const& request) {
	//check if the scene already exists
	if (mScenes.count(request.name)) {
		//delete the old scene
		printf("Scene \"%s\" already exists.\nOverwriting with new scene.\n", request.name.c_str());
		delete mScenes[request.name];
	}
	mScenes[request.name] = request.scene;
}

void SceneManager::processNavigateRequest(detail::SceneRequest const& request) {
	if (mScenes.find(request.name) != mScenes.end()) {

		//Cleanup the old scene
		if (mCurrentScene) {
			mCurrentScene->cleanup();
		}

		//Set the current scene
		mCurrentSceneName = request.name;
		mCurrentScene = mScenes[mCurrentSceneName];
	}
	else {
		printf("Navigation failed: Scene \"%s\" was not found.", request.name.c_str());
	}
}

SceneManager::~SceneManager() {
	for (auto& sceneEntry : mScenes) {
		delete sceneEntry.second;
	}

	mStopRequestThread = true;
	mRequestPending.notify_all();	// Tell the waiting thread to continue execution, so that we can join it
	_ASSERT(mRequestThread.joinable());
	mRequestThread.join();
}

std::unique_ptr<SceneManager> const& SceneManager::instance() {
	if (mInstance == nullptr) {
		mInstance = std::unique_ptr<SceneManager>(new SceneManager());
	}

	return mInstance;
}

std::string SceneManager::getCurrentScene() {
	std::lock_guard<std::mutex> lock(mSceneMutex);
	return mCurrentSceneName;
}

//calls the current scene's update method
void SceneManager::updateCurrentScene( sf::Time const &elapsedTime) {
	std::lock_guard<std::mutex> lock(mSceneMutex);
	if (mCurrentScene) {
		mCurrentScene->update(elapsedTime);
	}
}

//calls the current scene's draw method
void SceneManager::drawCurrentScene( sf::RenderWindow &w ) {
	std::lock_guard<std::mutex> lock(mSceneMutex);
	if (mCurrentScene) {
		mCurrentScene->draw(w);
	}
}

//returns true if no further processing should be done for the event (i.e. the event has been used up )
bool SceneManager::passEventToCurrentScene( sf::Event &theEvent ) {
	std::lock_guard<std::mutex> lock(mSceneMutex);
	bool handled = false;
	if (mCurrentScene) {
		handled = mCurrentScene->handleEvent(theEvent);
	}
	return handled;
}

void SceneManager::navigateToScene(std::string const &path ) {
	std::unique_lock<std::mutex> lock(mRequestMutex);

	//Add the request to the queue. (SceneRequest has a converting ctor for navigation requests)
	mRequests.push(path);

	mRequestPending.notify_all();
}