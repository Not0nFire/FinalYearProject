#ifndef _HERO_H
#define _HERO_H

#include <include/Pawn.hpp>

class Hero : public Pawn {
protected:
	//Ability mAbility;
public:
	Hero(sf::Texture& texture);
	~Hero();

	virtual void onCollide(Collidable* other, sf::Vector2f const &mtv) override;
};
#endif