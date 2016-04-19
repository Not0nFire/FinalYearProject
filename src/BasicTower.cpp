#include <include/Towers/BasicTower.h>
#include <include/ArcProjectile.h>

using namespace tower;


ProjectileTower::ProjectileTower(sf::Vector2f const &position, tinyxml2::XMLElement *xmlDef) :
Tower(position, xmlDef),
mTargetingSortPredicate([](std::weak_ptr<Pawn> &A_weak, std::weak_ptr<Pawn> &B_weak)
{	
	auto A = A_weak.lock();
	auto B = B_weak.lock();
	//if(A is alive but B is dead)
	if (!A->isDead() && B->isDead()) {
		return true;
	}
	else {
		//Sort by armour (lowest first)
		return A->getArmour() < B->getArmour();
	}
})//end sort predicate
{
	mRange = atof(xmlDef->FirstChildElement("Range")->GetText());
	mDamage = atof(xmlDef->FirstChildElement("Damage")->GetText());
	
	std::string damageTypeStr = xmlDef->FirstChildElement("DamageType")->GetText();

	if (damageTypeStr == "PHYSICAL") {
		mDamageType = Damage::Type::PHYSICAL;
	} else if (damageTypeStr == "MAGICAL") {
		mDamageType = Damage::Type::MAGICAL;
	} else {
		throw "Invalid DamageType in tower definition";
	}

	auto spawnOffset = xmlDef->FirstChildElement("ProjectileSpawnOffset");
	mProjectileSpawnOffset = sf::Vector2f(
		atof(spawnOffset->Attribute("x")),
		atof(spawnOffset->Attribute("y"))
	);
}

ProjectileTower::~ProjectileTower() {
	//empty dtor body
}

bool ProjectileTower::shoot(std::shared_ptr<std::list<std::shared_ptr<Pawn>>> const& possibleTargets) {
	bool targetAqcuired = false;
	if (mSecondsSinceLastAttack >= mSecondsPerAttack) {

		for (auto &p : *possibleTargets) {

			float distance = thor::length(p->getPosition() - (this->getPosition()+mProjectileSpawnOffset));

			//If p is not dead, and p is in range, and p is an enemy
			if (!p->isDead() && distance <= mRange && p->getFaction() == Pawn::Faction::ENEMY) {

				//Add it to the target list
				mTargetList.emplace_back(p);
			}
		}

		//Remove dead or distant targets from the list
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
			float distance = thor::length(chosenTarget->getPosition() - this->getPosition());

			auto projectile = std::make_shared<ArcProjectile>(mDamage, mDamageType, ResourceManager<sf::Texture>::instance()->get("./res/img/projectile.png"));

			float ttl = distance / mRange;

			//Fire the newly created projectile at the target.
			projectile->fire(getPosition() + mProjectileSpawnOffset, leadTarget(chosenTarget.get(), ttl), ttl);

			//Give the projectile to the manager. We lost ownership of it.
			mProjectileManager->give(projectile);

			mSecondsSinceLastAttack = 0.f;
			targetAqcuired = true;
		}
	}

	return targetAqcuired;
}

void ProjectileTower::setProjectileManager(std::shared_ptr<ProjectileManager> projectileMgr) {
	mProjectileManager = projectileMgr;
}

sf::Vector2f ProjectileTower::leadTarget(Pawn* target, float time) const {
	auto bounds = target->getGlobalBounds();
	sf::Vector2f prediction = target->getPosition() + sf::Vector2f(0.f, bounds.height * 0.5f);

	//only lead if target is moving
	if (target->getState() == Pawn::MARCHING) {
		sf::Vector2f heading = thor::unitVector(target->getDestination() - prediction);
		prediction = prediction + (heading * (target->getMovementSpeed() * time));
	}
	return prediction;
	//return target->getPosition() + (thor::unitVector(target->getDestination() - target->getPosition()) * (target->getMovementSpeed() * time));
}