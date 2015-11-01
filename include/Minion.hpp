#ifndef _MINION_H
#define _MINION_H

#include <include/Pawn.hpp>

class Minion : public Pawn {
public:
	enum Faction {
		ENEMY, PLAYER, NEUTRAL
	};
protected:
	Faction mFaction;

	//void calculateBehaviour();	//requires chai script
public:

	Minion(sf::Texture &texture, Faction _faction);
	~Minion();
};
#endif