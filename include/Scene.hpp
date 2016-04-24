#ifndef _SCENE_H
#define _SCENE_H

#include <memory>
#include <include/TinyXML2/tinyxml2.h>
#include <SFML\Graphics.hpp>
#include <functional>
#include <mutex>

/*!
\brief Interface for scene objects, such as levels and menus.
*/
class I_Scene {
public:
	/*!
	\param Event The event to be handled.
	\returns True if the event has been processed, false if the event is not processed or required.
	*/
	virtual bool handleEvent( sf::Event &Event ) = 0;

	/*!
	\param elapsedTime The amount of time that has past since the last time update was called.
	*/
	virtual void update( sf::Time const &elapsedTime ) = 0;

	/*!
	Should draw the scene to the renderwindow
	\param w The RenderWindow that the scene should be drawn to.
	*/
	virtual void draw( sf::RenderWindow &w ) = 0;

	virtual void cleanup() = 0;

	virtual ~I_Scene() {}
};

/*
\brief Proxy for holding scenes. Implements lazy loading.
Scene is only constructed when a method is called on it; It can be constructed prematurely by calling loadNow().
*/
//template <class SceneType>
class SceneProxy {
public:
	~SceneProxy();

	/*
	\brief Calls handleEvent on the scene object.
	Constructs the scene if it does not already exist.
	*/
	bool handleEvent(sf::Event &event);

	/*
	\brief Calls update on the scene object.
	Constructs the scene if it does not already exist.
	\param elapsedTime The amount of time since the last update.
	*/
	void update(sf::Time const &elapsedTime);

	/*
	\brief Calls handleEvent on the scene object.
	Constructs the scene if it does not already exist.
	\param w The RenderWindow to draw to.
	*/
	void draw(sf::RenderWindow &w);

	void cleanup();

	/*
	\brief Calls handleEvent on the scene object.
	Constructs the scene if it does not already exist.
	\remarks This is public so that the scene may be constructed before any calls need to be made to it.
	         This may be beneficial for scenes that require a relatively long time to load (e.g. Levels).
	*/
	void loadNow();

	//! Gets a pointer to the contained scene.
	I_Scene* getScene();

	/*!
	\brief Constructs a SceneProxy, whose contained scene can be constructed by the file specified by xmlPath.
	Creates a lambda function that constructs a new instance of SceneType using the xmlPath, this function is then
	called later on when the scene needs to be constructed.
	\param xmlPath Path to the xml file used to create the scene.
	*/
	template<class SceneType>
	static SceneProxy* create(std::string const& xmlPath);

private:
	SceneProxy(std::function<I_Scene*()> const& sceneCreationFunc);

	//! Actual scene object owned by proxy
	I_Scene* mScene;

	//! The location of the xml file used to create the scene when needed
	//std::string mXmlPath;

	std::function<I_Scene*()> mCreatSceneFunc;

	std::mutex mMutex;
};

template <class SceneType>
SceneProxy* SceneProxy::create(std::string const& xmlPath) {
	return new SceneProxy(
		[xmlPath] //capture string by copy
		() //lambda expression takes no arguments
		{
			tinyxml2::XMLDocument doc;

			tinyxml2::XMLError result = doc.LoadFile(xmlPath.c_str());	//try to load the xml from file
			if (result != tinyxml2::XML_NO_ERROR)
				throw result;	//throw an error if one occured

			tinyxml2::XMLElement* root = doc.FirstChildElement();	//root node

			return new SceneType(root);
		}//end lambda
	);//end return()
}
#endif

