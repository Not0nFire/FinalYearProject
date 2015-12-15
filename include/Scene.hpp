#ifndef _SCENE_H
#define _SCENE_H

#include <SFML\Graphics.hpp>

/*!
Interface for scene objects, such as levels and menus.
\remark I_Scene derivatives should never be instantiated outside of SceneManager class.
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
#endif

