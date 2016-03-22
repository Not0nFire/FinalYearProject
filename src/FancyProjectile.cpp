#include "include/FancyProjectile.hpp"

FancyProjectile::FancyProjectile(int damage, Damage::Type damageType, sf::Texture& texture) :
Projectile(damage, damageType, texture),
mSpeed(100.f),
mTurnSpeed(rand() % 40 + 80)
{
	auto bounds = getLocalBounds();
	setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);

	mEmitter.setParticleLifetime(thor::Distributions::uniform(sf::seconds(0.1f), sf::seconds(1.f)));
	mEmitter.setEmissionRate(50.f);
	mEmitter.setParticleRotation(thor::Distributions::uniform(0.f, 360.f));
	mEmitter.setParticleRotationSpeed(thor::Distributions::uniform(1.f, 10.f));
	mEmitter.setParticleVelocity(thor::Distributions::deflect(sf::Vector2f(10.f, 0.f), 360.f));
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

		if (nullptr != mTarget){
			if (mTarget->isDead()) {
				mTarget = nullptr;
			}
			else {
				mTargetPosition = mTarget->getMask()->getPosition();
			}
		}

		auto displacement = mTargetPosition - getPosition();
		auto distance = thor::length(displacement);
		auto elapsedSeconds = elapsedTime.asSeconds();
		mTimeToLive -= elapsedSeconds;

		if (distance < 5.f || mTimeToLive <= 0.f) {
			if (mTimeToLive <= 0.f) {
				mActive = false;
			}
			updateCollidableMask(getPosition());
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
			mEmitter.setParticlePosition(getPosition());
		}
		
	}//end if(active)
}

void FancyProjectile::setTarget(Pawn* newTarget) {
	mTargetPosition = newTarget->getPosition();
	mTarget = newTarget;
}

void FancyProjectile::attachToParticleSystem(thor::ParticleSystem& system) {
	assert(!mConnection.isConnected());

	mConnection = thor::ScopedConnection(system.addEmitter(thor::refEmitter(mEmitter)));
}
