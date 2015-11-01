#ifndef _SCENE_MANAGER_H
#define _SCENE_MANAGER_H

#include <map>
#include <include/Scene.hpp>

//singleton class to handle scenes and the navigation between them
class SceneManager {
private:
	std::map<std::string, I_Scene*> mScenes;

	std::string mCurrentScene;

	static SceneManager *mInstance;

	SceneManager() {}

public:
	~SceneManager() {}

	static SceneManager* instance();

	// Gets the name of the current scene.
	const std::string getCurrentScene() const;

	//get a pointer the the current scene (which allows it to be edited)
	I_Scene * getEditableScene() const;

	template<typename T>
	void createScene(std::string const &name, T* derivedSceneObject, bool goToScene = true);

	// Calls the current scene's update method
	void updateCurrentScene( sf::Time const &elapsedTime);

	// Calls the current scene's draw method
	void drawCurrentScene( sf::RenderWindow &w );

	// Returns true if no further processing should be done for the event (i.e. the event has been used up )
	bool passEventToCurrentScene( sf::Event &theEvent );

	bool navigateToScene( std::string const &path );
};

template<typename T>
void SceneManager::createScene(std::string const &name, T* derivedSceneObject, bool goToScene) {

	mScenes[name] = derivedSceneObject;
	if (goToScene) {
		mCurrentScene = name;
	}
}
#endif