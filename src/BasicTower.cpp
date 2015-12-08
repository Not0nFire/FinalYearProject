#include <include/Towers/BasicTower.h>
#include <include/ResourceManager.hpp>

using namespace tower;

BasicTower::BasicTower(sf::Texture &texture, sf::Vector2f position, float range, float attacksPerSecond, int damage, Damage::Type damageType) :
Actor(texture, new sf::CircleShape(40, 4), sf::Vector2f(-20.0f, 10.0f)),
mRange(range),
mAttacksPerSecond(attacksPerSecond),
mDamage(damage),
mDamageType(damageType),
mProjectile(10,
			Damage::Type::PHYSICAL,
			ResourceManager<sf::Texture>::instance()->get("././res/img/projectile.png")
			)
{
	setPosition(position);
}

BasicTower::~BasicTower() {}

void BasicTower::update(sf::Time const& elapsedTime) {
	mProjectile.update(elapsedTime);
}

void BasicTower::draw(sf::RenderTarget& target) {
	target.draw(*this);
	if (mProjectile.isActive()) {
		target.draw(mProjectile);
	}
}

bool BasicTower::acquireTarget(std::vector<Pawn*> const& possibleTargets) {
	bool targetAqcuired = false;
	if (!mProjectile.isActive()) {
		for (Pawn* p : possibleTargets) {
			if (!p->isDead() && thor::length(p->getPosition() - this->getPosition()) <= mRange && p->getFaction() == Pawn::Faction::ENEMY) {
				mProjectile.fire(getPosition(), p->getPosition(), 2);
				targetAqcuired = true;
				break;
			}//if (range)
		}//(pawn*)
	}//if (active)

	return targetAqcuired;
}