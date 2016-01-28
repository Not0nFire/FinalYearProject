#include <include/Minion.hpp>

Minion::Minion(sf::Texture& texture, Faction faction, Path const &pathToFollow) :
Pawn(texture, faction),
mPathNode(pathToFollow.begin())
{
	setDestination(mPathNode->getPoint());
}

Minion::Minion(PawnDef const& def, sf::Vector2f const &position, Path const &pathToFollow) :
Pawn(def),
mPathNode(pathToFollow.begin())
{
	setPosition(position);
}

Minion::~Minion() {
	
}

void Minion::update(sf::Time const& elapsedTime) {
	//Travel along path.
	if (thor::length(getDestination() - getPosition()) <= 20.f) {
		Node * nextNode = mPathNode->getNext();
		if (nextNode) {
			mPathNode = nextNode;
		}
		setDestination(mPathNode->getPoint());
	}

	Pawn::update(elapsedTime);
}
