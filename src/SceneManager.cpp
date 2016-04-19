#include <include/SceneManager.hpp>
#include <include/Settings.hpp>
#include <include/Camera.hpp>

std::unique_ptr<SceneManager> SceneManager::mInstance = nullptr;

SceneManager::SceneManager() :
mTranslateMouseEvents(true)
{
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

			if (mStopRequestThread) {
				return;
			}
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

		case detail::SceneRequest::ADD_AND_NAVIGATE:
			processAddRequest(request);
			processNavigateRequest(request);
			break;

		case detail::SceneRequest::SHOW_DIALOGUE:
			processDialogueRequest(request);
			break;

		}//end switch(type)

		mRequests.pop();
	}//end while true
}

void SceneManager::processAddRequest(detail::SceneRequest const& request) {
	std::cout << "[Add request]" << std::endl;
	//check if the scene already exists
	if (mScenes.count(request.name)) {
		//delete the old scene
		printf("Scene \"%s\" already exists.\nOverwriting with new scene.\n", request.name.c_str());
		//delete mScenes[request.name];
	}
	mScenes[request.name] = std::unique_ptr<SceneProxy>(request.scene);
}

void SceneManager::processNavigateRequest(detail::SceneRequest const& request) {
	std::cout << "[Navigate request]" << std::endl;
	if (mScenes.find(request.name) != mScenes.end()) {

		//Cleanup the old scene
		if (mCurrentScene) {
			mCurrentScene->cleanup();
		}

		//Set the current scene
		mCurrentSceneName = request.name;
		mCurrentScene = mScenes[mCurrentSceneName].get();
	}
	else {
		printf("Navigation failed: Scene \"%s\" was not found.", request.name.c_str());
	}
}

void SceneManager::processDialogueRequest(detail::SceneRequest const& request) {
	std::cout << "[Dialogue request]" << std::endl;
	_ASSERT(nullptr == mActiveDialogue);
	mActiveDialogue = request.dialogue;
}

SceneManager::~SceneManager() {
	mStopRequestThread = true;
	if (mRequestThread.joinable()) {
		mRequestPending.notify_all();	// Tell the waiting thread to continue execution, so that we can join it
		mRequestThread.join();
	}

	for (auto& entry : mScenes) {
		entry.second->cleanup();
	}
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
	if (!mActiveDialogue && mCurrentScene) {
		mCurrentScene->update(elapsedTime);
	}
}

//calls the current scene's draw method
void SceneManager::drawCurrentScene( sf::RenderWindow &w ) {
	std::lock_guard<std::mutex> lock(mSceneMutex);
	w.setView(Camera::getDefaultCamera());

	//draw scene if we have one
	if (mCurrentScene) {
		mCurrentScene->draw(w);
	}

	//draw dialogue box, in front of scene, if we have one
	if (mActiveDialogue) {
		w.draw(*mActiveDialogue);
	}
}

inline void translateMouseEvent(sf::Event &evnt) {
	//adjust mouse position to match window scaling
	if (evnt.type == sf::Event::EventType::MouseButtonPressed) {
		auto adjusted = Camera::getDefaultCamera().screenPositionToGamePosition(evnt.mouseButton.x, evnt.mouseButton.y);
		evnt.mouseButton.x = adjusted.x;
		evnt.mouseButton.y = adjusted.y;
	}
	else if (evnt.type == sf::Event::EventType::MouseMoved) {
		auto adjusted = Camera::getDefaultCamera().screenPositionToGamePosition(evnt.mouseMove.x, evnt.mouseMove.y);
		evnt.mouseMove.x = adjusted.x;
		evnt.mouseMove.y = adjusted.y;
	}
}

//returns true if no further processing should be done for the event (i.e. the event has been used up )
bool SceneManager::passEventToCurrentScene( sf::Event &theEvent ) {
	std::lock_guard<std::mutex> lock(mSceneMutex);
	bool handled = false;

	//pass event to dialogue box if one is open
	if (mActiveDialogue) {
		translateMouseEvent(theEvent);
		handled = mActiveDialogue->handleEvent(theEvent);

		//set our pointer to null when the box closes
		if (!mActiveDialogue->isOpen()) {
			mActiveDialogue = nullptr;
		}

	}
	else if (mCurrentScene) {

		if (mTranslateMouseEvents) {
			translateMouseEvent(theEvent);
		}
		handled = mCurrentScene->handleEvent(theEvent);
	}
	return handled;
}

void SceneManager::navigateToScene(std::string const &path ) {
	std::unique_lock<std::mutex> lock(mRequestMutex);

	std::cout << "-[Adding navigation request]=" << std::endl;

	//Add the request to the queue. (SceneRequest has a converting ctor for navigation requests)
	mRequests.push(path);

	mRequestPending.notify_all();

	mTranslateMouseEvents = true;

	std::cout << "-[Added navigation request]=" << std::endl;
}

void SceneManager::stopTranslatingMouse() {
	mTranslateMouseEvents = false;
}

void SceneManager::showDialogueBox(gui::DialogueBox* dialogueBox) {
	std::unique_lock<std::mutex> lock(mRequestMutex);

	std::cout << "-[Adding dialogue request]=" << std::endl;

	dialogueBox->open();

	//Default-construct a request. (Request type is DUMMY, so it will do nothing in this state)
	detail::SceneRequest request;

	//Set the request type
	request.type = detail::SceneRequest::SHOW_DIALOGUE;

	//Create the scene proxy
	request.dialogue = dialogueBox;

	//Add the request to the queue.
	mRequests.push(request);

	mRequestPending.notify_all();

	std::cout << "-[Added dialogue request]=" << std::endl;
}

void SceneManager::destruct() {
	delete mInstance.release();
}
