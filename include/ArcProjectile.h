#ifndef ARC_PROJECTILE_H
#define ARC_PROJECTILE_H

#include "Projectile.h"

//! Object that follows a pre-computed arc toward a target and checks for collision on impact.
class ArcProjectile : public Projectile {
public:
	ArcProjectile(int damage, Damage::Type damageType, sf::Texture& texture);
	virtual ~ArcProjectile();

	/*!
	Fires the projectile in a computed arc.
	\param from The location to fire from.
	\param to The destination of the projectile.
	\param flightTimeSeconds How much time the projectile should take to reach its destination.
	*/
	virtual void fire(sf::Vector2f const &from, sf::Vector2f const &to, float flightTimeSeconds) override;

	void update(sf::Time const& elapsedTime) override;

private:
	static const sf::Vector2f mGravity;
};
#endif
