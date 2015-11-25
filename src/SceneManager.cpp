#include "../include/SceneManager.hpp"

SceneManager* SceneManager::mInstance = nullptr;

SceneManager::SceneManager() {
	
}

SceneManager::~SceneManager() {
	for (std::pair<std::string, I_Scene*> sceneEntry : mScenes) {
		delete sceneEntry.second;
	}
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
		onSceneChange(mScenes[mCurrentScene]);
	}

	return success;
}

void SceneManager::createScene(std::string const &name, I_Scene* derivedSceneObject, bool goToScene) {

	mScenes[name] = derivedSceneObject;
	if (goToScene) {
		mCurrentScene = name;
	}
}