#ifndef _COLLIDABLE_H
#define _COLLIDEABLE_H

#include <SFML/Graphics/ConvexShape.hpp>
#include <boost/signals2.hpp>
#include <boost/thread/mutex.hpp>

using namespace boost::signals2;

class Collidable {
private:
	sf::ConvexShape mMask;
	signal<void(Collidable* other)> onCollide;

	boost::mutex mMutex;

public:
	Collidable(sf::ConvexShape const &mask);
	virtual ~Collidable();

	//remember to lock_guard the mutex when writing these methods!
	const sf::ConvexShape& getMask() const;
	void setMask(sf::ConvexShape mask);

	void addOnCollide( void(*func)(Collidable*) );
	void removeOnCollide(void(*func)(Collidable*));

	void onCollision(Collidable* other);
};
#endif