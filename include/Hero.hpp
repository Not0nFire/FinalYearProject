#ifndef _HERO_H
#define _HERO_H

#include <include/Pawn.hpp>
#include <include/ResourceManager.hpp>

/*!
\brief Pawn that can move and attack simultaneously and regenerates health.
*/
class Hero : public Pawn {
protected:

	//! Override of Pawn::doAttack that allows Hero to attack and move at the same time.
	virtual void doAttack(float secondsElapsed) override;

	//! Counter of update ticks
	unsigned int mTicks;
	//! Number of ticks before mHealthRegen is applied.
	const unsigned int M_TICKS_PER_REGEN;
	//! Amount of health to regenerate
	unsigned mHealthRegen;

	//Ability mAbility;
public:
	Hero(tinyxml2::XMLElement* xml);
	~Hero();

	//! Heals mHealthRegen amount every M_TICKS_PER_REGEN ticks.
	void update(sf::Time const& elapsedTime) override;

	virtual void onCollide(Collidable* other, sf::Vector2f const &mtv) override;
};
#endif