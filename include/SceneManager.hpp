#ifndef SCENE_MANAGER_HPP
#define SCENE_MANAGER_HPP

#include <include/Scene.hpp>
#include <include/Gui/DialogueBox.hpp>
#include <functional>
#include <map>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <queue>

namespace detail
{
	struct SceneRequest {
		//!Converting ctor for easy navigation requests
		SceneRequest(std::string const& _name) : type(NAVIGATE), name(_name), scene(nullptr) {}
		SceneRequest() : type(DUMMY) {}
		enum Type {
			DUMMY,
			NAVIGATE,
			ADD_TO_MANAGER,
			ADD_AND_NAVIGATE,
			SHOW_DIALOGUE
		} type;

		std::string name;	//!< Name of scene

		//No request should ever have both a scene and dialogue
		union {
			SceneProxy* scene;	//!< Scene to add to the manager
			gui::DialogueBox* dialogue;	//<! Dialog box to be shown
		};
	};
}

/*!
\brief Singleton class to handle scenes and the navigation between them.
*/
class SceneManager {
private:
	/*!
	A map of all scenes managed by the SceneManager.
	Each scene is identified by its name.
	*/
	std::map<std::string, std::unique_ptr<SceneProxy>> mScenes;

	gui::DialogueBox* mActiveDialogue;

	//! The current scenes map key.
	std::string mCurrentSceneName;
	SceneProxy* mCurrentScene;

	static std::unique_ptr<SceneManager> mInstance;

	//! Must be set to true before we try and join switcher thread.
	std::atomic_bool mStopRequestThread;

	/*! Thread for handling scene requests.
	\see handleSceneRequests()
	*/
	std::thread mRequestThread;
	std::mutex mSceneMutex;	//!< Locked whenever we need to do something involving the current scene.
	std::mutex mRequestMutex;	//!< Locked whenever we need to access the request queue.
	std::condition_variable mRequestPending;	//!< Notified whenever a request is pushed onto the queue.
	std::queue<detail::SceneRequest> mRequests;	//!< Queue of pending requests

	SceneManager();

	//! Waits to be notified about a pending scene change, then does cleanup and assignment.
	void handleSceneRequests();

	//! Adds a scene to the map
	void processAddRequest(detail::SceneRequest const& request);

	//! Transitions to a scene
	void processNavigateRequest(detail::SceneRequest const& request);

	//! Show a dialog box on top of the current scene
	void processDialogueRequest(detail::SceneRequest const& request);

public:
	~SceneManager();

	static std::unique_ptr<SceneManager> const& instance();

	/*!
	Gets the name of the current scene (its key in the map of scenes)
	\returns The name of the current scene.
	*/
	std::string getCurrentScene();

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
	void updateCurrentScene( sf::Time const &elapsedTime );

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
	void navigateToScene( std::string const &path );

	void showDialogueBox(gui::DialogueBox* dialogueBox);

	//! Destroys the instance.
	static void destruct();
};

template <class SceneType>
void SceneManager::createScene(std::string const& name, std::string const& xmlPath, bool goToScene) {
	std::unique_lock<std::mutex> lock(mRequestMutex);

	//Construct the scene from the name. It defaults to a navigation request.
	detail::SceneRequest request = name;

	//Decide whether we need to just add the scene or if we should navigate to it too.
	request.type = goToScene ?
		detail::SceneRequest::ADD_AND_NAVIGATE :
		detail::SceneRequest::ADD_TO_MANAGER;

	//Create the scene proxy
	request.scene = SceneProxy::create<SceneType>(xmlPath);

	//Add the request to the queue.
	mRequests.push(request);

	mRequestPending.notify_all();
}
#endif
