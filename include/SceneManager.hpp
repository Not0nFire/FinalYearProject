#ifndef _SCENE_MANAGER_H
#define _SCENE_MANAGER_H

#include <include/Scene.hpp>
#include <boost/signals2.hpp>
#include <map>
using namespace boost::signals2;

//singleton class to handle scenes and the navigation between them
class SceneManager {
private:
	std::map<std::string, I_Scene*> mScenes;

	std::string mCurrentScene;

	static SceneManager *mInstance;

	SceneManager();

public:
	~SceneManager();

	static SceneManager* instance();

	// Gets the name of the current scene.
	std::string getCurrentScene() const;

	//get a pointer the the current scene (which allows it to be edited)
	I_Scene * getEditableScene() const;

	void createScene(std::string const &name, I_Scene* derivedSceneObject, bool goToScene = true);

	// Calls the current scene's update method
	void updateCurrentScene( sf::Time const &elapsedTime);

	// Calls the current scene's draw method
	void drawCurrentScene( sf::RenderWindow &w );

	// Returns true if no further processing should be done for the event (i.e. the event has been used up )
	bool passEventToCurrentScene( sf::Event &theEvent );

	bool navigateToScene( std::string const &path );

	signal<void(std::string)> onSceneChange;
};
#endif