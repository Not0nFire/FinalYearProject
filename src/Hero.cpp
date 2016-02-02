#include <include/Hero.hpp>

Hero::Hero(tinyxml2::XMLElement* xml) :
Pawn(xml->FirstChildElement("Pawn"))
{
	_ASSERT(std::string(xml->Name()) == "Hero");

	playAnimation("idle", true);
}

Hero::~Hero() {
}

void Hero::doAttack(float secondsElapsed) {
	//allow the player to attack while moving
	Pawn::doAttack(secondsElapsed);
	doMarch(getDestination() - getPosition(), secondsElapsed);
}

void Hero::onCollide(Collidable* other, sf::Vector2f const& mtv) {
	collision::Collidable::onCollide(other, mtv);	//call base method
}
