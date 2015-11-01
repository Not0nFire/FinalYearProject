#include <include/Collision/Collidable.hpp>

namespace collision {

	Collidable::Collidable(sf::Shape* mask, sf::Vector2f offset)
		: mMask(mask), mOffset(offset) {
		(*mMask).setFillColor(sf::Color::Magenta);
	}

	Collidable::~Collidable() {
	}

	void Collidable::updateCollidableMask(sf::Vector2f newPosition) {
		(*mMask).setPosition(newPosition + mOffset);
	}

	const sf::Shape* Collidable::getMask() const {
		return mMask;
	}

	void Collidable::setMask(sf::Shape* mask, sf::Vector2f offset) {
		mMask = mask;
		mOffset = offset;
	}

	void Collidable::debug_draw(sf::RenderTarget& target) {
		target.draw(*mMask);
	}
}//namespace