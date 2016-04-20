#include "include/Towers/MageTower.hpp"

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

	if (mSecondsSinceLastAttack >= mSecondsPerAttack) {

		for (auto &p : *possibleTargets) {

			float distance = thor::length(p->getPosition() - this->getPosition());

			//If p is not dead, and p is in range, and p is an enemy
			if (!p->isDead() && distance <= mRange && p->getFaction() == Pawn::Faction::ENEMY) {

				//Add it to the target list
				mTargetList.emplace_back(p);
			}
		}

		//Remove dead or distance targets from the list
		auto itr = mTargetList.begin();
		while (mTargetList.end() != itr) {
			if (itr->expired() || thor::length(itr->lock()->getPosition() - this->getPosition()) > mRange) {
				itr = mTargetList.erase(itr);
			}
			else {
				++itr;
			}
		}

		//If there's a target we can attack...
		if (!mTargetList.empty()) {

			//Sort the targets
			mTargetList.sort(mTargetingSortPredicate);

			auto const& chosenTarget = mTargetList.begin()->lock();

			auto projectile = std::make_shared<FancyProjectile>(mDamage, mDamageType, ResourceManager<sf::Texture>::instance()->get("./res/img/projectile.png"));

			//Fire the newly created projectile at the target.
			projectile->fire(getPosition() + mProjectileSpawnOffset, chosenTarget->getPosition(), 5.f);

			projectile->setTarget(chosenTarget);

			//Give the projectile to the manager. We lost ownership of it.
			mProjectileManager->give(projectile);

			mSecondsSinceLastAttack = 0.f;
			targetAqcuired = true;
		}
	}

	return targetAqcuired;
}
