#include <include/Hero.hpp>

Hero::Hero(tinyxml2::XMLElement* xml) :
Pawn(xml->FirstChildElement("Pawn")),
mSecondsSinceRegen(0.f),
M_HEALTH_REGEN(atoi(xml->FirstChildElement("HealthRegen")->GetText())),
M_REGEN_INTERVAL(1.f / atof(xml->FirstChildElement("HealthRegen")->Attribute("rate")))
{
	_ASSERT(std::string(xml->Name()) == "Hero");

	playAnimation("idle", true);
}

Hero::~Hero() {
}

void Hero::update(sf::Time const& elapsedTime) {
	mSecondsSinceRegen += elapsedTime.asSeconds();
	if (mState != DEAD && mSecondsSinceRegen >= M_REGEN_INTERVAL)
	{
		mSecondsSinceRegen -= M_REGEN_INTERVAL;
		heal(M_HEALTH_REGEN);
	}

	Pawn::update(elapsedTime);

	turnToFaceDirection(mDestination);

}

void Hero::doAttack(float secondsElapsed) {
	//allow the player to attack while moving
	Pawn::doAttack(secondsElapsed);
	doMarch(getDestination() - getPosition(), secondsElapsed);
}
