#include <include/Hero.hpp>

Hero::Hero(sf::Texture &texture) : Pawn(texture, Faction::PLAYER){
	mMovementSpeed = 100;
	mHealth = 500;
	mAttackDamage = 20;
	mAttacksPerSecond = 2.0f;
	mArmour = Damage::Reduction{ Damage::Reduction::LIGHT };
	mMagicResist = Damage::Reduction{ Damage::Reduction::MEDIUM };
}

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

void Hero::calculateAnimation() {
	switch (mState)
	{
	case IDLE: 
		if (isPlayingAnimation() && getPlayingAnimation() != "idle")
			playAnimation("idle", true);
		break;
	case MARCHING:
		if (isPlayingAnimation() && getPlayingAnimation() != "walk")
			playAnimation("walk", true);
		break;
	case ATTACKING:
		if (isPlayingAnimation() && getPlayingAnimation() != "attack")
			playAnimation("attack", true);
		break;
	case STUNNED: break;
	case DEAD: break;
	default: break;
	}
}

void Hero::onCollide(Collidable* other, sf::Vector2f const& mtv) {
	collision::Collidable::onCollide(other, mtv);	//call base method
}
