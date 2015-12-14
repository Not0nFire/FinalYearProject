#ifndef _COLLIDABLE_H
#define _COLLIDABLE_H

#include <SFML/Graphics.hpp>

namespace collision {

	class Collidable {
	private:
		sf::Shape* mMask; /*!< The shape that is used to test collision. */
		//signal<void(Collidable* other)> onCollide;
		sf::Vector2f mOffset;

	protected:
		/*!
		\brief Updates the position of the mask.
		Updates the position of the mask to equal newPosition plus the offset.
		\param The new position of the mask, before offset is taken into account.
		*/
		void updateCollidableMask(sf::Vector2f newPosition);

	public:
		Collidable(sf::Shape* mask, sf::Vector2f offset);
		virtual ~Collidable();

		const sf::Shape* getMask() const;
		void setMask(sf::Shape* mask, sf::Vector2f offset);

		/*!
		\brief Called when we collide with something.
		*/
		virtual void onCollide(Collidable* other, sf::Vector2f const &mtv);

		/*void addOnCollide(void(*func)(Collidable*));
		void removeOnCollide(void(*func)(Collidable*));

		virtual void onCollision(Collidable* other);
		virtual void onCollision(Collidable* other, sf::Vector2f mtv);*/

		void setDebugColour(sf::Color const &c);
		void debug_draw(sf::RenderTarget& target);

		/*!
		\brief Gets all axies of the mask.
		Gets all axies of the mask.
		Currently inefficient due to the fact that it iterates over all points and returns all axies (a duplicate of each axis is included!).
		\param axisVector[in,out] The container of axies to add our axies to.
		*/
		void getAxies(std::vector<sf::Vector2f> &axiesVector);

		/*!
		\brief Gets the number of points in the mask.
		Calls mask->getPoint() and returns the result.
		*/
		size_t getPointCount() const;

		/*!
		\brief Projects the mask onto an axis.
		Projects each point of the mask onto an axis and returns the min and max points it covers on that axis.
		\remarks May try to normalize zero length vectors.
		\param axis The axis to project onto.
		\returns The shadow of the mask along the axis. i.e. The min and max points it touches on the axis.
		*/
		sf::Vector2f projectOntoAxis(sf::Vector2f &axis) const;
	};
}
#endif