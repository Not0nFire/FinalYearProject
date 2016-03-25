#ifndef _SCENE_MANAGER_H
#define _SCENE_MANAGER_H

#include <include/Scene.hpp>
#include <boost/signals2.hpp>
#include <map>
#include <thread>
using namespace boost::signals2;

/*!
\brief Singleton class to handle scenes and the navigation between them.
*/
class SceneManager {
private:
	/*!
	A map of all scenes managed by the SceneManager.
	Each scene is identified by its name.
	*/
	std::map<std::string, SceneProxy*> mScenes;

	std::string mCurrentScene;

	static SceneManager *mInstance;

	/*!
	\brief Thread used solely for calling cleanup() on old scene when changing to a new scene.
	This allows scenes to call navigate on the SceneManager from inside a scene, without deadlocking or throwing system_errors
	from trying to lock a mutex while it is currently owned by the same thread.
	*/
	std::thread mCleanupThread;

	void joinCleanupThread();

	SceneManager();

public:
	~SceneManager();

	static SceneManager* instance();

	/*!
	Gets the name of the current scene (its key in the map of scenes)
	\returns The name of the current scene.
	*/
	std::string getCurrentScene() const;

	/*!
	Gets a pointer to the current scene, allowing it to be edited.
	Use getCurrentScene() in place of this method wherever possible.
	\returns A pointer to the current scene.
	*/
	//I_Scene * getEditableScene() const;

	/*!
	Adds a scene to the map under the specified name.
	\param name The name of the scene (its key in the map)
	\param derivedSceneObject The scene to be added.
	\param goToScene True if the new scene should become the current scene (i.e. navigated to immediately)
	*/
	template <class SceneType>
	void createScene(std::string const &name, std::string const& xmlPath, bool goToScene = true);

	/*!
	Calls the current scene's update method.
	*/
	void updateCurrentScene( sf::Time const &elapsedTime);

	/*!
	Calls the current scene's draw method
	*/
	void drawCurrentScene( sf::RenderWindow &w );

	/*!
	Calls handleEvent on the current scene, passing theEvent.
	\param theEvent The event that will be passes to the current scene.
	\returns True if the event has been processed and no further processing of this event is required. (i.e. event is used up)
	*/
	bool passEventToCurrentScene( sf::Event &theEvent );

	/*!
	Sets the current scene to be path.
	\param path The name of the scene to be navigated to.
	\returns True if the path matched an existing scene.
	*/
	bool navigateToScene( std::string const &path );

	/*!
	Signal that is invoked whenever the scene changes.
	The scene pointer passed is the new current scene.
	*/
	signal<void(SceneProxy* newScene)> onSceneChange;
};

template <class SceneType>
void SceneManager::createScene(std::string const& name, std::string const& xmlPath, bool goToScene) {

	mScenes[name] = SceneProxy::create<SceneType>(xmlPath);
	if (goToScene) {
		navigateToScene(name);
	}
}
#endif
