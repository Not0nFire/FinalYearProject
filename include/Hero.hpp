#ifndef _HERO_H
#define _HERO_H

#include <include/Pawn.hpp>
#include <include/ResourceManager.hpp>

class Hero : public Pawn {
protected:

	virtual void doAttack(float secondsElapsed) override;

	unsigned int mTicks;
	const unsigned int mTicksPerRegen;
	unsigned mHealthRegen;

	//Ability mAbility;
public:
	Hero(tinyxml2::XMLElement* xml);
	~Hero();

	void update(sf::Time const& elapsedTime) override;

	virtual void onCollide(Collidable* other, sf::Vector2f const &mtv) override;
};
#endif