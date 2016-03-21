#include <include/Projectile.h>

Projectile::Projectile(int damage, Damage::Type damageType, sf::Texture& texture) :
Actor(texture, new sf::CircleShape(10, 5), sf::Vector2f(-5.0f, -5.0f)),
mDamage(damage),
mDamageType(damageType),
mActive(false),
mOnHit([](Projectile* p){ std::cout << "Projectile::mOnHit invoked, but no function set." << std::endl; })
{
	auto bounds = getLocalBounds();
	setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);
}

Projectile::~Projectile() {
	
}

bool Projectile::isActive() const {
	return mActive;
}

void Projectile::setOnHit(std::function<void(Projectile*)> onHit) {
	mOnHit = onHit;
}

int Projectile::getDamage() const {
	return mDamage;
}

Damage::Type Projectile::getDamageType() const {
	return mDamageType;
}

void Projectile::onCollide(Collidable* other, sf::Vector2f const& mtv) {
	Pawn* pawn = dynamic_cast<Pawn*>(other);
	if (pawn && pawn->getFaction() == Pawn::Faction::ENEMY) {
		pawn->takeDamage(mDamage, mDamageType);
		printf("Projectile Hit! %p\n", other);
		mActive = false;
	}
}