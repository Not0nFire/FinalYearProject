#include <include/Towers/BasicTower.h>
#include <include/ResourceManager.hpp>

using namespace tower;

const int BasicTower::mCost = 100;

BasicTower::BasicTower(sf::Texture &texture, sf::Vector2f position, float range, float attacksPerSecond, int damage, Damage::Type damageType, collision::CollisionGroup* projectileCollisionGroup) :
Actor(texture,

//define points of shape (there HAS to be a better way!)
[]()
{
	sf::ConvexShape* mask = new sf::ConvexShape(4u);
	mask->setPoint(0u, sf::Vector2f(0.f, -25.f));
	mask->setPoint(1u, sf::Vector2f(-55.f, 0.f));
	mask->setPoint(2u, sf::Vector2f(0.f, 25.f));
	mask->setPoint(3u, sf::Vector2f(55.f, 0.f));
	return mask;
}(),

sf::Vector2f(0.0f, 3.0f)),
mRange(range),
mAttacksPerSecond(attacksPerSecond),
mDamage(damage),
mDamageType(damageType),
mProjectile(10,
			Damage::Type::PHYSICAL,
			ResourceManager<sf::Texture>::instance()->get("./res/img/projectile.png")
),
mProjectileSpawnOffset(0.f, -80.f)
{
	auto bounds = getLocalBounds();
	setOrigin(bounds.width * .5f, bounds.height * 0.85f);
	setPosition(position);
	
	updateCollidableMask(getPosition());

	printf("tower: %f, %f. mask: %f, %f.", getPosition().x, getPosition().y, getMask()->getPosition().x, getMask()->getPosition().y);

	//Tell the projectile to check for collision when it hits
	mProjectile.connectOnHit(bind(&collision::CollisionGroup::checkAgainst, projectileCollisionGroup, _1));
}

BasicTower::~BasicTower() {}

void BasicTower::update(sf::Time const& elapsedTime) {
	mProjectile.update(elapsedTime);
}

void BasicTower::draw(sf::RenderTarget& target) {
	target.draw(*this);
	if (mProjectile.isActive()) {
		target.draw(mProjectile);
		mProjectile.debug_draw(target);
	}
}

bool BasicTower::acquireTarget(std::list<Pawn*> const& possibleTargets) {
	bool targetAqcuired = false;
	if (!mProjectile.isActive()) {
		for (Pawn* p : possibleTargets) {
			if (!p->isDead() && thor::length(p->getPosition() - this->getPosition()) <= mRange && p->getFaction() == Pawn::Faction::ENEMY) {
				mProjectile.fire(getPosition() + mProjectileSpawnOffset, leadTarget(p, 2.f), 2.f);
				targetAqcuired = true;
				break;
			}//if (range)
		}//(pawn*)
	}//if (active)

	return targetAqcuired;
}

int BasicTower::getCost() {
	return mCost;
}

sf::Vector2f BasicTower::leadTarget(Pawn* target, float time) const {
	sf::Vector2f prediction = target->getPosition();

	//only lead if target is moving
	if (target->getState() == Pawn::MARCHING) {
		sf::Vector2f heading = thor::unitVector(target->getDestination() - prediction);
		prediction = prediction + (heading * (target->getMovementSpeed() * time));
	}
	return prediction;
	//return target->getPosition() + (thor::unitVector(target->getDestination() - target->getPosition()) * (target->getMovementSpeed() * time));
}
