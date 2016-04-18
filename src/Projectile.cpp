#include <include/Projectile.h>

Projectile::Projectile(int damage, Damage::Type damageType, sf::Texture& texture, int impactRadius) :
Actor(texture, new sf::CircleShape(impactRadius, 5), sf::Vector2f(-impactRadius, -impactRadius)),
mDamage(damage),
mDamageType(damageType),
mActive(false),
mImpactOccurred(false)
{
	auto bounds = getLocalBounds();
	setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);
}

Projectile::~Projectile() {
	
}

bool Projectile::isActive() const {
	return mActive;
}

bool Projectile::impactOccured() const {
	return mImpactOccurred;
}

int Projectile::getDamage() const {
	return mDamage;
}

Damage::Type Projectile::getDamageType() const {
	return mDamageType;
}

void Projectile::onCollide(std::shared_ptr<Collidable> &other, sf::Vector2f const& mtv) {
	auto pawn = std::dynamic_pointer_cast<Pawn, Collidable>(other);
	if (pawn && pawn->getFaction() == Pawn::Faction::ENEMY) {
		pawn->takeDamage(mDamage, mDamageType);
		printf("Projectile Hit! %p\n", other.get());
		mActive = false;
	}
}
