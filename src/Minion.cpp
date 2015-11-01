#include <include/Minion.hpp>

Minion::Minion(sf::Texture& texture, Faction _faction) : Pawn(texture), mFaction(_faction) {
	mMovementSpeed = 50;
}

Minion::~Minion() {
	
}