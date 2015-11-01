#include "../include/SceneManager.hpp"

SceneManager* SceneManager::mInstance = nullptr;

SceneManager* SceneManager::instance() {
	if (mInstance == NULL) {
		mInstance = new SceneManager();
	}

	return mInstance;
}

const std::string SceneManager::getCurrentScene() const {
	return mCurrentScene;
}

I_Scene * SceneManager::getEditableScene() const {
	return mScenes.at(mCurrentScene);
}

//calls the current scene's update method
void SceneManager::updateCurrentScene( sf::Time const &elapsedTime) {
	mScenes[ mCurrentScene ]->update( elapsedTime);
}

//calls the current scene's draw method
void SceneManager::drawCurrentScene( sf::RenderWindow &w ) {
	mScenes[ mCurrentScene ]->draw( w );
}

//returns true if no further processing should be done for the event (i.e. the event has been used up )
bool SceneManager::passEventToCurrentScene( sf::Event &theEvent ) {
	return mScenes[ mCurrentScene ]->handleEvent( theEvent );
}

bool SceneManager::navigateToScene(std::string const &path ) {
	bool success = false;

	if ( mScenes.find(path) != mScenes.end() ) {
		mCurrentScene = path;
		success = true;
	}

	return success;
}