#include <include/Towers/BasicTower.h>
#include <include/ResourceManager.hpp>

using namespace tower;

//Define cost of tower.
int BasicTower::mCost = 100;

//Define shape of mask.
sf::ConvexShape BasicTower::mPlacementMask = [](){
sf::ConvexShape mask = sf::ConvexShape(4u);
mask.setPoint(0u, sf::Vector2f(0.f, -25.f));
mask.setPoint(1u, sf::Vector2f(-55.f, 0.f));
mask.setPoint(2u, sf::Vector2f(0.f, 25.f));
mask.setPoint(3u, sf::Vector2f(55.f, 0.f));
return std::move(mask);
}();


BasicTower::BasicTower(sf::Texture &texture, sf::Vector2f const &position, float range, float attacksPerSecond, int damage, Damage::Type damageType, std::shared_ptr<ProjectileManager> projectileMgr) :
Actor(texture,
      new sf::ConvexShape(BasicTower::getMask()),
      sf::Vector2f(0.0f, 3.0f)
     ),
mRange(range),
mSecondsPerAttack(1.f / attacksPerSecond),
mSecondsSinceLastAttack(0.f),
mDamageType(damageType),
mDamage(damage),
mProjectileSpawnOffset(0.f, -80.f),
mProjectileManager(projectileMgr)
{
	auto bounds = getLocalBounds();
	setOrigin(bounds.width * .5f, bounds.height * 0.85f);
	setPosition(position);
	
	updateCollidableMask(getPosition());

	printf("tower: %f, %f. mask: %f, %f.", getPosition().x, getPosition().y, Collidable::getMask()->getPosition().x, Collidable::getMask()->getPosition().y);
}

BasicTower::~BasicTower() {}

void BasicTower::update(sf::Time const& elapsedTime) {
	mSecondsSinceLastAttack += elapsedTime.asSeconds();
}

void BasicTower::draw(sf::RenderTarget& target) {
	target.draw(*this);
}

bool BasicTower::acquireTarget(std::list<Pawn*> const& possibleTargets) {
	bool targetAqcuired = false;
	if (mSecondsSinceLastAttack >= mSecondsPerAttack) {

		for (auto p : possibleTargets) {

			float distance = thor::length(p->getPosition() - this->getPosition());

			//If p is not dead, and p is in range, and p is an enemy
			if (!p->isDead() && distance <= mRange && p->getFaction() == Pawn::Faction::ENEMY) {

				auto projectile = std::make_unique<FancyProjectile>(mDamage, mDamageType, ResourceManager<sf::Texture>::instance()->get("./res/img/projectile.png"));

				projectile->setTarget(p);

				float ttl = distance / mRange;

				//Fire the newly created projectile at the target.
				projectile->fire(getPosition() + mProjectileSpawnOffset, leadTarget(p, ttl), ttl);

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

int BasicTower::getCost() {
	return mCost;
}

sf::ConvexShape BasicTower::getMask() {
	return mPlacementMask;
}

sf::Vector2f BasicTower::leadTarget(Pawn* target, float time) const {
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
