#include <include/Hero.hpp>

Hero::Hero(tinyxml2::XMLElement* xml) :
Pawn(xml->FirstChildElement("Pawn")),
mSecondsSinceRegen(0.f),
mHealthRegen(atoi(xml->FirstChildElement("HealthRegen")->GetText()))
{
	_ASSERT(std::string(xml->Name()) == "Hero");

	playAnimation("idle", true);
}

Hero::~Hero() {
}

void Hero::update(sf::Time const& elapsedTime) {
	mSecondsSinceRegen += elapsedTime.asSeconds();
	if (mState != DEAD && mSecondsSinceRegen >= 1.f)
	{
		mSecondsSinceRegen -= 1.f;
		heal(mHealthRegen);
	}

	Pawn::update(elapsedTime);

	turnToFaceDirection(mDestination);

}

void Hero::doAttack(float secondsElapsed) {
	//allow the player to attack while moving
	Pawn::doAttack(secondsElapsed);
	doMarch(getDestination() - getPosition(), secondsElapsed);
}
