#include <include/Collision/Collidable.hpp>
#include <Thor/Vectors.hpp>
#include <iostream>

namespace collision {

	Collidable::Collidable(sf::Shape* mask, sf::Vector2f offset)
		: mMask(mask), mOffset(offset) {
		(*mMask).setFillColor(sf::Color::Magenta);
	}

	Collidable::~Collidable() {
		delete mMask;
	}

	void Collidable::updateCollidableMask(sf::Vector2f newPosition) {
		mMask->setPosition(newPosition + mOffset);
	}

	const sf::Shape* Collidable::getMask() const {
		return mMask;
	}

	void Collidable::setMask(sf::Shape* mask, sf::Vector2f offset) {
		delete mMask;
		mMask = mask;
		mOffset = offset;
	}

	void Collidable::onCollide(Collidable* other, sf::Vector2f const &mtv) {
		mMask->setFillColor(sf::Color::Red);
	}

	void Collidable::setDebugColour(sf::Color const &c) {
		mMask->setFillColor(c);
	}

	void Collidable::debug_draw(sf::RenderTarget& target) {
		target.draw(*mMask);
	}

	void Collidable::getAxies(std::vector<sf::Vector2f>& axiesVector) {
		int pointCount = mMask->getPointCount();
		for (int i = 0; i < pointCount; ++i)
		{
			sf::Vector2f p1 = mMask->getPoint(i);
			sf::Vector2f p2 = mMask->getPoint(i + 1 == pointCount ? 0 : i + 1);
			sf::Vector2f edge = p1 - p2;
			//sf::Vector2f normal = sf::Vector2f(-edge.y, edge.x);
			axiesVector.push_back(sf::Vector2f(-edge.y, edge.x));
		}
	}

	size_t Collidable::getPointCount() const {
		return mMask->getPointCount();
	}

	sf::Vector2f Collidable::projectOntoAxis(sf::Vector2f & axis) const {
		if (thor::length(axis) != 1)
		{
			//normalize the axis
			axis = thor::unitVector(axis);
		}

		float min = thor::dotProduct(axis, mMask->getPosition() + mMask->getPoint(0));
		float max = min;
		float p;

		size_t pointCount = mMask->getPointCount();
		for (size_t i = 1; i < pointCount; ++i)
		{
			p = thor::dotProduct(axis, mMask->getPosition() + mMask->getPoint(i));

			if (p < min)
			{
				min = p;
			}
			else if (p > max)
			{
				max = p;
			}
		}

		return sf::Vector2f(min, max);
	}
}//namespace