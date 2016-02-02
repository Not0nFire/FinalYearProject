#include <include/Minion.hpp>

Minion::Minion(tinyxml2::XMLElement* xml) :
Pawn(xml->FirstChildElement("Pawn"))
{
	_ASSERT(std::string(xml->Name()) == "Minion");
}

Minion::~Minion() {
	
}

void Minion::setPath(const Node* pathNode) {
	mPathNode = pathNode;
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
