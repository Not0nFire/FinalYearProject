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

	float mSecondsSinceRegen;

	//! Seconds to wait between applying health regen
	const float M_REGEN_INTERVAL;

	//! Amount of health to regenerate per second
	const unsigned int M_HEALTH_REGEN;

	//Ability mAbility;
public:
	Hero(tinyxml2::XMLElement* xml);
	~Hero();

	void update(sf::Time const& elapsedTime) override;
};
#endif