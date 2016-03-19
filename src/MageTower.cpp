#include "include/Towers/MageTower.hpp"

using namespace tower;

MageTower::MageTower(sf::Texture& texture, sf::Vector2f const& position, float range, float attacksPerSecond, int damage, std::shared_ptr<ProjectileManager> projectileManager) :
BasicTower(texture, position, range, attacksPerSecond, damage, Damage::Type::MAGICAL, projectileManager)
{
}

MageTower::~MageTower() {
}

bool MageTower::acquireTarget(std::list<Pawn*> const& possibleTargets) {
	bool targetAqcuired = false;
	if (mSecondsSinceLastAttack >= mSecondsPerAttack) {

		for (Pawn* p : possibleTargets) {

			//If p is not dead, and p is in range, and p is an enemy
			if (!p->isDead() && thor::length(p->getPosition() - this->getPosition()) <= mRange && p->getFaction() == Pawn::Faction::ENEMY) {

				//Construct a new unique projectile from the prototype
				auto projectile = std::make_unique<FancyProjectile>(mDamage, mDamageType, ResourceManager<sf::Texture>::instance()->get("./res/img/projectile.png"));

				//Fire the newly created projectile at the target.
				projectile->fire(getPosition() + mProjectileSpawnOffset, leadTarget(p, 1.f), 1.f);

				//Give the projectile to the manager. We lost ownership of it.
				mProjectileManager->give(std::move(projectile));

				mSecondsSinceLastAttack = 0.f;
				targetAqcuired = true;
				break;

			}//if (range)

		}//(pawn*)

	}

	return targetAqcuired;
}
