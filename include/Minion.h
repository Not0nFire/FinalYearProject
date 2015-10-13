#ifndef _MINION_H
#define _MINION_H

#include <include/Pawn.h>

class Minion : public Pawn {
protected:
	enum Faction;
	Faction mFaction;

	//void calculateBehaviour();	//requires chai script -- hardcode until it's implemented
public:
	enum Faction {
		ENEMY, PLAYER, NEUTRAL
	};

	Minion(Faction _faction);
	~Minion();
};
#endif