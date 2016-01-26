#ifndef _ACTOR_H
#define _ACTOR_H

#include <SFML/Graphics.hpp>
#include <include/Collision/Collidable.hpp>
#include <boost/thread/mutex.hpp>
#include <include/TinyXML2/tinyxml2.h>
#include <include/ResourceManager.hpp>

/*!
\brief Class for anything that animates and has a position.
Animates and has position and collision.
A lot is yet to be implemented. Will make heavy used of the thor::Animator class on itself.
\remarks Used for things like Pawns, Minions, Towers, etc...
*/
class Actor : public sf::Sprite, public collision::Collidable {
protected:
	bool mVisible;
	boost::mutex mMutex;	/*!< Not to be confused with the private mutex in Collidable */
public:
	Actor(sf::Texture &texture, sf::Shape* collisionMask, sf::Vector2f const &maskOffset);
	Actor(tinyxml2::XMLElement* element);
	virtual ~Actor();

	void setVisible(bool isVisible = true);
	bool toggleVisible();
	bool getVisible() const;
};
#endif