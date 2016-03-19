#include "include/FancyProjectile.hpp"

FancyProjectile::FancyProjectile(int damage, Damage::Type damageType, sf::Texture& texture) :
Projectile(damage, damageType, texture),
mSpeed(100.f),
mTurnSpeed(rand() % 40 + 80)
{
	auto bounds = getLocalBounds();
	setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);
}

FancyProjectile::~FancyProjectile() {
	
}

void FancyProjectile::fire(sf::Vector2f const& from, sf::Vector2f const& to, float flightTimeSeconds) {
	setPosition(from);

	auto displacement = to - from;
	auto direction = -(thor::unitVector(displacement));

	mVelocity = direction * mSpeed;
	setRotation(thor::toDegree(atan2f(direction.x, direction.y)));

	mTimeToLive = 5.f;
	mActive = true;
}

template <typename T>
inline T perpDot(const sf::Vector2<T>& A, const sf::Vector2<T>& B)
{
	return (A.x * B.y) - (A.y * B.x);
}

void FancyProjectile::update(sf::Time const& elapsedTime) {
	if (mActive) {
		using thor::unitVector;
		using thor::dotProduct;
		using thor::toRadian;

		if (nullptr != mTarget && mTarget->isDead()) {
			mTarget = nullptr;
		} else {
			mTargetPosition = mTarget->getPosition();
		}

		auto displacement = mTargetPosition - getPosition();
		auto distance = thor::length(displacement);
		auto elapsedSeconds = elapsedTime.asSeconds();
		mTimeToLive -= elapsedSeconds;

		if (distance < 10.f || mTimeToLive <= 0.f) {
			updateCollidableMask(getPosition());
			mActive = false;
			mOnHit(this);
		}
		else {

			//auto currentDirection = unitVector(mVelocity);
			auto desiredDirection = unitVector(displacement);
			auto currentDirection = unitVector(mVelocity);

			float perpDotProd = perpDot(currentDirection, desiredDirection);

			if (perpDotProd > 0.f) {
				rotate(mTurnSpeed * elapsedSeconds);
			}
			else if (perpDotProd < 0.f) {
				rotate(-mTurnSpeed * elapsedSeconds);
			} 
			else {	// perpDotProduct is 0 and we are either facing directly at or away from our target
				if (dotProduct(currentDirection, desiredDirection) < 0.f) {	//if dotProduct is negative we are facing away from target
					rotate(mTurnSpeed * elapsedSeconds);
				}
			}

			float rotationRadians = toRadian(getRotation());
			mVelocity.x = cosf(rotationRadians);
			mVelocity.y = sinf(rotationRadians);

			mVelocity = unitVector(mVelocity) * mSpeed;

			move(mVelocity * elapsedSeconds);
		}
	}//end if(active)
}

void FancyProjectile::setTarget(Pawn* newTarget) {
	mTargetPosition = newTarget->getPosition();
	mTarget = newTarget;
}
