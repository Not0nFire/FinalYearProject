#ifndef FANCY_PROJECTILE_HPP
#define FANCY_PROJECTILE_HPP

#include "Projectile.h"
#include <Thor/Particles.hpp>
#include <Thor/Math.hpp>
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>

/*!
\class FancyProjectile
\brief Projectile that emits particles as it travels and homes in on its target.
*/
class FancyProjectile : public Projectile {
public:
	/*!
	\brief Constructs a fancy projectile.
	\param damage The amount of damage to inflict on impact.
	\param damageType The type of damage in inflict, PHYSICAL or MAGICAL.
	\param texture Visual representation of the projectile.
	*/
	FancyProjectile(int damage, Damage::Type damageType, sf::Texture& texture, int impactRadius = 10);
	virtual ~FancyProjectile();

	/*!
	\brief Fires a homing projectile in the opposite direction of the target.
	The projectiles initial velocity shoots it away from its target,
	but the projectile then turns around, homing in on its target.
	\param from The starting location.
	\param to The destination location.
	\param Ignored by this projectile type.
	*/
	void fire(sf::Vector2f const& from, sf::Vector2f const& to, float flightTimeSeconds) override;
	
	/*!
	\brief Moves the projectile toward its destination.
	Moves the projectile and associated emitter toward its destination, accounting for turning circle.
	\param elapsedTime How much time has passed since update() was last called.
	*/
	void update(sf::Time const& elapsedTime) override;

	void setTarget(std::shared_ptr<Pawn> const &newTarget);

	void attachToParticleSystem(thor::ParticleSystem &system);

private:
	//!	Connection to a thor::ParticleSystem. Upon destruction, mEmitter will be disconnected.
	thor::ScopedConnection mConnection;

	//! Emitter used to make particles.
	thor::UniversalEmitter mEmitter;

	const float mSpeed;

	//! How much the projectile can turn (in radians) each update.
	const float mTurnSpeed;

	std::weak_ptr<Pawn> mTarget;
};
#endif