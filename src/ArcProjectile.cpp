#include "include/ArcProjectile.h"

sf::Vector2f const ArcProjectile::mGravity = sf::Vector2f(0, 100);

ArcProjectile::ArcProjectile(int damage, Damage::Type damageType, sf::Texture& texture) :
Projectile(damage, damageType, texture) {
	//empty ctor body
}

ArcProjectile::~ArcProjectile() {
	//empty dtor body
}

void ArcProjectile::fire(sf::Vector2f const &from, sf::Vector2f const &to, float flightTimeSeconds) {
	mWasFired = true;
	setPosition(from);

	//	S = ut + 0.5at^2
	//	U = (S - (0.5)at^2) / t
	//	U = S - (0.5)at
	mVelocity = (to - from - (0.5f)*mGravity*powf(flightTimeSeconds, 2)) / flightTimeSeconds;

	mTimeToLive = flightTimeSeconds;
	mActive = true;
}

void ArcProjectile::update(sf::Time const& elapsedTime) {
	if (mActive) {
		float elapsedSeconds = elapsedTime.asSeconds();
		mVelocity += mGravity * elapsedSeconds;
		move(mVelocity * elapsedSeconds);
		setRotation(thor::toDegree(atan2f(mVelocity.y, mVelocity.x)));
		mTimeToLive -= elapsedSeconds;

		//if we've reached our target
		if (mTimeToLive <= 0) {
			updateCollidableMask(getPosition());
			mImpactOccurred = true;
			mActive = false;
		}
	}//if (mActive)
}