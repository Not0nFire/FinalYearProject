#include "include/Towers/MageTower.hpp"

using namespace tower;

MageTower::MageTower(sf::Vector2f const &position, tinyxml2::XMLElement *xmlDef) :
ProjectileTower(position, xmlDef)
{
}

MageTower::~MageTower() {
}

bool MageTower::shoot(std::shared_ptr<std::list<std::shared_ptr<Pawn>>> const& possibleTargets) {
	bool targetAqcuired = false;
	if (mSecondsSinceLastAttack >= mSecondsPerAttack) {

		for (auto &p : *possibleTargets) {

			//If p is not dead, and p is in range, and p is an enemy
			if (!p->isDead() && thor::length(p->getPosition() - this->getPosition()) <= mRange && p->getFaction() == Pawn::Faction::ENEMY) {

				//Construct a new unique projectile from the prototype
				auto projectile = std::make_shared<FancyProjectile>(mDamage, mDamageType, ResourceManager<sf::Texture>::instance()->get("./res/img/magic_projectile.png"));

				//Fire the newly created projectile at the target.
				projectile->fire(getPosition() + mProjectileSpawnOffset, p->getPosition(), 5.f);

				projectile->setTarget(p);

				//Give the projectile to the manager. We lost ownership of it.
				mProjectileManager->give(projectile);

				mSecondsSinceLastAttack = 0.f;
				targetAqcuired = true;
				break;

			}//if (range)

		}//(pawn*)

	}

	return targetAqcuired;
}
