#include "include/Towers/MageTower.hpp"
#include <include/Constants.h>

using namespace tower;

MageTower::MageTower(sf::Vector2f const &position, tinyxml2::XMLElement *xmlDef) :
ProjectileTower(position, xmlDef)
{
	mTargetingSortPredicate = [](std::weak_ptr<Pawn> &A_weak, std::weak_ptr<Pawn> &B_weak)
	{	
		if (auto A = A_weak.lock()) {
			if (auto B = B_weak.lock()) {
				//if(A is alive but B is dead)
				if (!A->isDead() && B->isDead()) {
					return true;	//prioritise live prey
				}
				else {
					//Sort by magic resist (lowest first)
					return A->getMagicResist() < B->getMagicResist();
				}
			}
		}
		return false;
	};
}

MageTower::~MageTower() {
}

bool MageTower::shoot(std::shared_ptr<std::list<std::shared_ptr<Pawn>>> const& possibleTargets) {
	bool targetAqcuired = false;
	auto currentNumTargets = 0u;
	static const auto maxNumTargets = Constants::Numbers::getMageTowerMaxTargets();
	if (mSecondsSinceLastAttack >= mSecondsPerAttack) {

		for (auto &p : *possibleTargets) {
			if (currentNumTargets >= maxNumTargets) {
				break;
			}

			//If p is not dead, and p is in range, and p is an enemy
			if (!p->isDead() && thor::length(p->getPosition() - this->getPosition()) <= mRange && p->getFaction() == Pawn::Faction::ENEMY) {

				//Construct a new unique projectile from the prototype
				auto projectile = std::make_shared<FancyProjectile>(mDamage, mDamageType, ResourceManager<sf::Texture>::instance()->get("./res/img/magic_projectile.png"));

				//Fire the newly created projectile at the target.
				projectile->fire(getPosition() + mProjectileSpawnOffset, p->getPosition(), 5.f);

				projectile->setTarget(p);

				//Give the projectile to the manager. We lost ownership of it.
				mProjectileManager->give(projectile);

				++currentNumTargets;

				if (!targetAqcuired) {
					mSecondsSinceLastAttack = 0.f;
					targetAqcuired = true;
				}

			}//if (range)

		}//(pawn*)

	}

	return targetAqcuired;
}
