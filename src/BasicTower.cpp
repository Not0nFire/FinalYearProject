#include <include/Towers/BasicTower.h>

using namespace tower;


ProjectileTower::ProjectileTower(sf::Vector2f const &position, tinyxml2::XMLElement *xmlDef) :
Tower(position, xmlDef)
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

bool ProjectileTower::shoot(std::list<std::shared_ptr<Pawn>> const& possibleTargets) {
	bool targetAqcuired = false;
	if (mSecondsSinceLastAttack >= mSecondsPerAttack) {

		for (auto p : possibleTargets) {

			float distance = thor::length(p->getPosition() - this->getPosition());

			//If p is not dead, and p is in range, and p is an enemy
			if (!p->isDead() && distance <= mRange && p->getFaction() == Pawn::Faction::ENEMY) {

				auto projectile = std::make_shared<ArcProjectile>(mDamage, mDamageType, ResourceManager<sf::Texture>::instance()->get("./res/img/projectile.png"));

				float ttl = distance / mRange;

				//Fire the newly created projectile at the target.
				projectile->fire(getPosition() + mProjectileSpawnOffset, leadTarget(p.get(), ttl), ttl);

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
