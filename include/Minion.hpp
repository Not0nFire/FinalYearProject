#ifndef _MINION_H
#define _MINION_H

#include <include/Pawn.hpp>
#include <include/Pathing/Path.hpp>
#include <include/MinionCreation/PawnDef.hpp>

class Minion : public Pawn {
private:
	Node* mPathNode;

public:

	Minion(sf::Texture &texture, Faction faction, Path const &pathToFollow);
	Minion(PawnDef const &def, sf::Vector2f const &position, Path const &pathToFollow);
	virtual ~Minion();

	virtual void update(sf::Time const &elapsedTime) override;
};
#endif