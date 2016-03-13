#ifndef _SCENE_H
#define _SCENE_H

#include <memory>
#include <include/TinyXML2/tinyxml2.h>
#include <SFML\Graphics.hpp>

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
template <class SceneType>
class SceneProxy {
public:
	/*
	\brief Constructs a SceneProxy. Stores the xmlPath. Scene is NOT constructed here.
	\param xmlPath Path to xml file that will be used to load scene. Root node of file should be <Scene>.
	*/
	SceneProxy(const char * xmlPath);
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

	/*
	\brief Calls handleEvent on the scene object.
	Constructs the scene if it does not already exist.
	\remarks This is public so that the scene may be constructed before any calls need to be made to it.
	         This may be beneficial for scenes that require a relatively long time to load (e.g. Levels).
	*/
	void loadNow();

	//! Gets a pointer to the contained scene.
	SceneType* getScene() const;

private:
	//! Actual scene object owned by proxy
	std::unique_ptr<I_Scene> mScene;

	//! The location of the xml file used to create the scene when needed
	const char * mXmlPath;
};

template <class SceneType>
SceneProxy<SceneType>::SceneProxy(const char* xmlPath) :
mXmlPath(xmlPath)
{
	//empty ctor body
}

template <class SceneType>
SceneProxy<SceneType>::~SceneProxy() {
	//unique pointer to scene goes out of scope here, so scene will be deleted
}

template <class SceneType>
bool SceneProxy<SceneType>::handleEvent(sf::Event& sfEvent) {
	if (mScene == nullptr)
	{
		loadNow();
	}

	return mScene->handleEvent(sfEvent);
}

template <class SceneType>
void SceneProxy<SceneType>::update(sf::Time const& elapsedTime) {
	if (mScene == nullptr)
	{
		loadNow();
	}

	mScene->update(elapsedTime);
}

template <class SceneType>
void SceneProxy<SceneType>::draw(sf::RenderWindow& w) {
	if (mScene == nullptr)
	{
		loadNow();
	}

	mScene->draw(w);
}

template <class SceneType>
void SceneProxy<SceneType>::loadNow() {
	_ASSERT(mScene == nullptr);

	using namespace tinyxml2;

	XMLDocument doc;

	XMLError result = doc.LoadFile(mXmlPath);	//try to load the xml from file
	if (result != XML_NO_ERROR)
		throw result;	//throw an error if one occured

	XMLElement* root = doc.FirstChildElement();	//root node

	//construct the scene
	mScene = std::make_unique<SceneType>(root);
}

template <class SceneType>
SceneType* SceneProxy<SceneType>::getScene() const {
	return mScene.get();
}
#endif

