#ifndef _ACTOR_H
#define _ACTOR_H

#include <SFML/Graphics.hpp>
#include <include/Collision/Collidable.hpp>
#include <boost/thread/mutex.hpp>

//We can use this class for anything that animates and has collision: hero. minions, buttons, effects, abilities, etc...
class Actor : public sf::Sprite, public collision::Collidable {
protected:
	bool mVisible;
	boost::mutex mMutex;	//Not to be confused with the private mutex in Collidable
public:
	Actor(sf::Texture &texture, sf::Shape* collisionMask, sf::Vector2f const &maskOffset);
	virtual ~Actor();

	void setVisible(bool isVisible = true);
	bool toggleVisible();
	bool getVisible() const;
};
#endif