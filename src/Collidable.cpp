#include <include\Collidable.h>

Collidable::Collidable(sf::ConvexShape const &mask)
: mMask(mask) {
}

Collidable::~Collidable() {
}

const sf::ConvexShape& Collidable::getMask() const {
	return mMask;
}

void Collidable::setMask(sf::ConvexShape mask) {
	mMask = mask;
}