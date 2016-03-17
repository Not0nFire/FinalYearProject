#include "include/FancyProjectile.hpp"

FancyProjectile::FancyProjectile(int damage, Damage::Type damageType, sf::Texture& texture) :
Projectile(damage, damageType, texture),
mSpeed(100.f),
mTurnSpeed(3.f)
{
	auto bounds = getLocalBounds();
	setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);
}

FancyProjectile::~FancyProjectile() {
	
}

void FancyProjectile::fire(sf::Vector2f const& from, sf::Vector2f const& to, float flightTimeSeconds) {
	setPosition(from);
	mTarget = to;
	auto displacement = to - from;
	auto direction = (thor::unitVector(displacement));

	mVelocity = direction * mSpeed;
	setRotation(thor::toDegree(atan2f(direction.x, direction.y)));

	mTimeToLive = 5.f;
	mActive = true;
}

void FancyProjectile::update(sf::Time const& elapsedTime) {
	if (mActive) {
		using thor::unitVector;
		using thor::dotProduct;
		using thor::toDegree;
		using thor::toRadian;

		auto displacement = getPosition() - mTarget;
		auto distance = thor::length(displacement);
		auto elapsedSeconds = elapsedTime.asSeconds();
		mTimeToLive -= elapsedSeconds;
		if (distance < 1.f || mTimeToLive <= 0.f) {
			updateCollidableMask(getPosition());
			mActive = false;
			mOnHit(this);
		}
		else {

			

			//auto currentDirection = unitVector(mVelocity);
			auto desiredDirection = unitVector(mTarget - getPosition());

			auto desiredAngle = acosf(dotProduct(sf::Vector2f(1.f, 0.f), desiredDirection));
			auto currentAngle = toRadian(getRotation());

			if (currentAngle != desiredAngle) {
				auto deltaAngle = desiredAngle - currentAngle;

				// Just set current to be the desired angle if using turn speed would overshoot.
				if (abs(deltaAngle) < mTurnSpeed * elapsedSeconds) {
					currentAngle = desiredAngle;
				}
				else {

					// Keep it in range from -180 to 180 to make the most efficient turns.
					if (deltaAngle > M_PI) {
						deltaAngle -= M_PI_2;
					}
					else if (deltaAngle < -M_PI) {
						deltaAngle += M_PI_2;
					}

					if (deltaAngle > 0.f) {
						//turn clockwise
						currentAngle += mTurnSpeed * elapsedSeconds;
					}
					else {
						//turn counterclockwise
						currentAngle -= mTurnSpeed * elapsedSeconds;
					}

				}

				mVelocity.x = cosf(currentAngle);
				mVelocity.y = sinf(currentAngle);
				mVelocity *= mSpeed * elapsedSeconds;
				setRotation(toDegree(currentAngle));
			}//end if(current != desired)

			move(mVelocity);
		}
	}//end if(active)
}
