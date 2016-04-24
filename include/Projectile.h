#ifndef _PROJECTILE_H
#define _PROJECTILE_H

#include <include/Actor.hpp>
#include <include/Damage.hpp>
#include <Thor/Vectors.hpp>
#include "Pawn.hpp"

class Projectile : public Actor {
public:
	Projectile(int damage, Damage::Type damageType, sf::Texture& texture, int impactRadius = 10);
	~Projectile();

	/*!
	\param from The location to fire from.
	\param to The destination of the projectile.
	\param flightTimeSeconds How much time the projectile should take to reach its destination.
	*/
	virtual void fire(sf::Vector2f const &from, sf::Vector2f const &to, float ttl) = 0;

	virtual void update(sf::Time const &elapsedTime) = 0;

	bool isActive() const;

	bool impactOccured() const;

	virtual void onCollide(std::shared_ptr<Collidable> &other, sf::Vector2f const& mtv) override;

	int getDamage() const;

	Damage::Type getDamageType() const;

protected:
	bool mActive;
	sf::Vector2f mVelocity;
	float mTimeToLive;

	bool mImpactOccurred;

private:
	const Damage::Type mDamageType;
	const int mDamage;
};
#endif