#include <include/Projectile.h>

sf::Vector2f const Projectile::G = sf::Vector2f(0,100);

Projectile::Projectile(int damage, Damage::Type damageType, sf::Texture& texture) :
Actor(texture, new sf::CircleShape(10, 6), sf::Vector2f(0.0f, 0.0f)),
mDamage(damage),
mDamageType(damageType),
mActive(false),
mOnHit([](Projectile* p){ std::cout << "Projectile::mOnHit invoked, but no function set." << std::endl; })
{
	setOrigin(40.f, 2.f);
}

Projectile::~Projectile() {
	
}

// v = u + at
// s = ut + 0.5at^2
// s = 0.5(u+v)t
// v^2 = u^2 + 2as
// s = vt - 0.5at^2

void Projectile::fire(sf::Vector2f const &from, sf::Vector2f const &to, float flightTimeSeconds) {
	setPosition(from);
	mTarget = to;

	//	S = ut + 0.5at^2
	//	U = (S - (0.5)at^2) / t
	//	U = S - (0.5)at
	//sf::Vector2f S = to - from;
	//sf::Vector2f halfat2 = (0.5f)*G*powf(flightTimeSeconds, 2);
	mVelocity = (to - from - (0.5f)*G*powf(flightTimeSeconds, 2)) / flightTimeSeconds;
	
	mTimeToLive = flightTimeSeconds;
	mActive = true;
}

void Projectile::update(sf::Time const& elapsedTime) {
	if (mActive) {
		float elapsedSeconds = elapsedTime.asSeconds();
		mVelocity += G * elapsedSeconds;
		move(mVelocity * elapsedSeconds);
		setRotation(thor::toDegree(atan2f(mVelocity.y, mVelocity.x)));
		mTimeToLive -= elapsedSeconds;

		//if we've reached our target
		if (mTimeToLive <= 0) {
			updateCollidableMask(getPosition());
			mActive = false;
			mOnHit(this);
		}
	}//if (mActive)
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
		printf("Projectile Hit!\n");
		mActive = false;
	}
}