#include <include/Hero.hpp>

Hero::Hero(tinyxml2::XMLElement* xml) :
Pawn(xml->FirstChildElement("Pawn")),
mTicks(0u),
M_TICKS_PER_REGEN(atoi(xml->FirstChildElement("HealthRegen")->Attribute("rate"))),
mHealthRegen(atoi(xml->FirstChildElement("HealthRegen")->GetText()))
{
	_ASSERT(std::string(xml->Name()) == "Hero");

	playAnimation("idle", true);
}

Hero::~Hero() {
}

void Hero::update(sf::Time const& elapsedTime) {
	mTicks = (mTicks + 1) % UINT32_MAX;
	if (mState != DEAD && mTicks >= M_TICKS_PER_REGEN)
	{
		mTicks -= M_TICKS_PER_REGEN;
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
	Pawn::onCollide(other, mtv);	//call base method
}
