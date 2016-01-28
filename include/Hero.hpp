#ifndef _HERO_H
#define _HERO_H

#include <include/Pawn.hpp>

class Hero : public Pawn {
protected:

	virtual void doAttack(float secondsElapsed) override;

	//Ability mAbility;
public:
	Hero(sf::Texture& texture);
	Hero(PawnDef const &def, sf::Vector2f const &position);
	~Hero();

	virtual void onCollide(Collidable* other, sf::Vector2f const &mtv) override;
};
#endif