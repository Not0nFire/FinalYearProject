#ifndef TOWER_H
#define TOWER_H

#include <include/Actor.hpp>
#include <include/Pawn.hpp>

namespace tower
{
	/*!
	\class Tower
	\brief Interface for derived tower types.
	*/
	class Tower : public Actor {
	public:
		Tower(sf::Vector2f const &position, tinyxml2::XMLElement *xmlDef);
		virtual ~Tower();

		virtual void update(sf::Time const &elapsedTime);

		virtual bool shoot(std::list<std::shared_ptr<Pawn>> const &targetList) = 0;

		virtual int getCost() const;
	protected:
		const float mSecondsPerAttack;
		float mSecondsSinceLastAttack;
		const int mCost;
	};
}
#endif