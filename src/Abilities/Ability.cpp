#include <include/Abilities/Ability.hpp>

Ability::Ability(tinyxml2::XMLElement* xml) :
mDescription(xml->FirstChildElement("Description")->GetText()),
mName(xml->FirstChildElement("Name")->GetText())
{
	_ASSERT(xml->Name() == std::string("Ability"));
}

Ability::~Ability() {
}

void Ability::update(sf::Time const& elapsedTime) {
	if (mIsActive)
	{
		doUpdateLogic(elapsedTime.asSeconds());
	}
}

const std::string& Ability::getDescription() const{
	return mDescription;
}

const std::string& Ability::getName() const {
	return mName;
}

void Ability::setPawnList(std::shared_ptr<std::list<std::shared_ptr<Pawn>>> const& list) {
	mPawnList = list;
}

void Ability::setProjectileManager(std::shared_ptr<ProjectileManager> const& manager) {
	mProjectileManager = manager;
}

void Ability::activate() {
	mIsActive = true;
}

void Ability::deactivate() {
	mIsActive = false;
}

bool Ability::isActive() const {
	return mIsActive;
}

void Ability::spawnPawn(std::shared_ptr<Pawn> const& pawn) const {
	mPawnList->push_back(pawn);
}

void Ability::spawnProjectile(std::shared_ptr<Projectile> const& projectile) const {
	mProjectileManager->give(projectile);
}
