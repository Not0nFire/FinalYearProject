#ifndef _PROJECTILE_H
#define _PROJECTILE_H

#include <include/Actor.hpp>
#include <include/Damage.hpp>
//#include <boost/signals2.hpp>
#include <Thor/Vectors.hpp>
#include "Pawn.hpp"

//using namespace boost::signals2;

//! Object that follows a pro-computed arc toward a target and checks for collision on impact.
class Projectile : public Actor {
private:
	Damage::Type mDamageType;
	int mDamage;

	bool mActive;

	static const sf::Vector2f G;

	sf::Vector2f mTarget;
	sf::Vector2f mVelocity;
	float mTimeToLive;

	//signal<void(Projectile*)> onHit;

public:
	Projectile(int damage, Damage::Type damageType, sf::Texture& texture);
	~Projectile();

	/*!
	Fires the projectile in a computed arc.
	\param from The location to fire from.
	\param to The destination of the projectile.
	\param flightTimeSeconds How much time the projectile should take to reach its destination.
	*/
	virtual void fire(sf::Vector2f const &from, sf::Vector2f const &to, float flightTimeSeconds);

	virtual void update(sf::Time const &elapsedTime);

	bool isActive() const;

	void setOnHit(std::function<void(Projectile*)> onHit);

	virtual void onCollide(Collidable* other, sf::Vector2f const& mtv) override;

	int getDamage() const;

	Damage::Type getDamageType() const;

	std::function<void(Projectile*)> mOnHit;
};
#endif