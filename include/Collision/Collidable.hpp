#ifndef _COLLIDABLE_H
#define _COLLIDABLE_H

#include <SFML/Graphics.hpp>
#include <boost/signals2.hpp>

using namespace boost::signals2;
namespace collision {

	class Collidable {
	private:
		sf::Shape* mMask;
		//signal<void(Collidable* other)> onCollide;
		sf::Vector2f mOffset;

	protected:
		void updateCollidableMask(sf::Vector2f newPosition);

	public:
		Collidable(sf::Shape* mask, sf::Vector2f offset);
		virtual ~Collidable();

		const sf::Shape* getMask() const;
		void setMask(sf::Shape* mask, sf::Vector2f offset);

		virtual void onCollide(Collidable* other, sf::Vector2f const &mtv);

		/*void addOnCollide(void(*func)(Collidable*));
		void removeOnCollide(void(*func)(Collidable*));

		virtual void onCollision(Collidable* other);
		virtual void onCollision(Collidable* other, sf::Vector2f mtv);*/

		void debug_draw(sf::RenderTarget& target);

		void getAxies(std::vector<sf::Vector2f> &axiesVector);
		size_t getPointCount() const;
		sf::Vector2f projectOntoAxis(sf::Vector2f &axis) const;
	};
}
#endif