#include "../include/SceneManager.hpp"

SceneManager* SceneManager::mInstance = nullptr;

void SceneManager::joinCleanupThread() {
	if (mCleanupThread.joinable()) {
		mCleanupThread.join();
	}
}

SceneManager::SceneManager() {
	
}

SceneManager::~SceneManager() {
	//for (std::pair<std::string, I_Scene*> sceneEntry : mScenes) {
	//	delete sceneEntry.second;
	//}
}

SceneManager* SceneManager::instance() {
	if (mInstance == nullptr) {
		mInstance = new SceneManager();
	}

	return mInstance;
}

std::string SceneManager::getCurrentScene() const {
	return mCurrentScene;
}

//I_Scene * SceneManager::getEditableScene() const {
//	return mScenes.at(mCurrentScene);
//}

//calls the current scene's update method
void SceneManager::updateCurrentScene( sf::Time const &elapsedTime) {
	joinCleanupThread();
	mScenes[ mCurrentScene ]->update( elapsedTime);
}

//calls the current scene's draw method
void SceneManager::drawCurrentScene( sf::RenderWindow &w ) {
	joinCleanupThread();
	mScenes[ mCurrentScene ]->draw( w );
}

//returns true if no further processing should be done for the event (i.e. the event has been used up )
bool SceneManager::passEventToCurrentScene( sf::Event &theEvent ) {
	joinCleanupThread();
	return mScenes[ mCurrentScene ]->handleEvent( theEvent );
}

bool SceneManager::navigateToScene(std::string const &path ) {
	joinCleanupThread();

	bool success = false;

	if ( mScenes.find(path) != mScenes.end() ) {
		//cleanup old scene in seperate thread to avoid double locking the SceneProxy mutex in this thread
		if (!mCurrentScene.empty()) {
			mCleanupThread = std::thread(&SceneProxy::cleanup, mScenes[mCurrentScene]);
		}
		mCurrentScene = path;
		success = true;
		onSceneChange(mScenes[mCurrentScene]);
	}

	return success;
}