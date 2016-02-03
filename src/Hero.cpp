#include <include/Hero.hpp>

Hero::Hero(tinyxml2::XMLElement* xml) :
Pawn(xml->FirstChildElement("Pawn")),
mTicks(0u),
mTicksPerRegen(atoi(xml->FirstChildElement("HealthRegen")->Attribute("rate"))),
mHealthRegen(atoi(xml->FirstChildElement("HealthRegen")->GetText()))
{
	_ASSERT(std::string(xml->Name()) == "Hero");

	playAnimation("idle", true);
}

Hero::~Hero() {
}

void Hero::update(sf::Time const& elapsedTime) {
	mTicks = (mTicks + 1) % UINT32_MAX;
	if (mState != DEAD && mTicks >= mTicksPerRegen)
	{
		mTicks -= mTicksPerRegen;
		heal(mHealthRegen);
	}

	Pawn::update(elapsedTime);

}

void Hero::doAttack(float secondsElapsed) {
	//allow the player to attack while moving
	Pawn::doAttack(secondsElapsed);
	doMarch(getDestination() - getPosition(), secondsElapsed);
}

void Hero::onCollide(Collidable* other, sf::Vector2f const& mtv) {
	collision::Collidable::onCollide(other, mtv);	//call base method
}
