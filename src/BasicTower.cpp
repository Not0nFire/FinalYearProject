#include <include/Towers/BasicTower.h>
#include <include/ResourceManager.hpp>

using namespace tower;

BasicTower::BasicTower(sf::Texture &texture, sf::Vector2f position, float range, float attacksPerSecond, int damage, Damage::Type damageType) :
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

sf::Vector2f(0.0f, 0.0f)),
mRange(range),
mAttacksPerSecond(attacksPerSecond),
mDamage(damage),
mDamageType(damageType),
mProjectile(10,
			Damage::Type::PHYSICAL,
			ResourceManager<sf::Texture>::instance()->get("././res/img/projectile.png")
			)
{
	auto bounds = getLocalBounds();
	setOrigin(bounds.width * .05f, bounds.height * 0.85f);
	setPosition(position);

	
	updateCollidableMask(getPosition());

	printf("tower: %f, %f. mask: %f, %f.", getPosition().x, getPosition().y, getMask()->getPosition().x, getMask()->getPosition().y);
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