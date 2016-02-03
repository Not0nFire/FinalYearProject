#ifndef _BASIC_TOWER_H
#define _BASIC_TOWER_H

#include <include/Actor.hpp>
#include <include/Damage.hpp>
#include <include/Projectile.h>
#include <include/Pawn.hpp>
#include <include/Collision/CollisionGroup.hpp>


namespace tower
{
	/*!
	\brief Tower that fires a projectile toward a target.
	Takes account for moving targets.
	*/
	class BasicTower : public Actor {
		
		//! Radius that the tower can fire within.
		float mRange;
		//! Currently unused.
		float mAttacksPerSecond;

		//! The type of damage to inflick (PHYSICAL or MAGICAL)
		Damage::Type mDamageType;
		//! The amount of damage each projectile inflicts.
		int mDamage;

		//! Projectile that the tower fires.
		Projectile mProjectile;
		//! Relative position at which to fire the projectile from.
		sf::Vector2f mProjectileSpawnOffset;

		/*!
		\brief Predicts where a marching Pawn will be after a certain number of seconds.
		\returns A vector of where the Pawn will be if it keeps moving in its current direction.
		*/
		sf::Vector2f leadTarget(Pawn* target, float time) const;

		//! How much money the tower costs to place.
		static const int mCost;

	public:
		/*!
		\brief Consructs a Basic Tower.
		\param texture Visual representation of the tower.
		\param position Position of the tower in the Level.
		\param range Radius around the tower, within which it will fire upon units.
		\param attacksPerSecond CURRENTLY UNUSED
		\param damage Amount of damage each projectile inflicts.
		\param damageType The damageType of each projectile.
		\param projectileCollisionGroup Pointer to a collision group. Projectiles will be checked against this group when they land.
		*/
		BasicTower(sf::Texture &texture, sf::Vector2f position, float range, float attacksPerSecond, int damage, Damage::Type damageType, collision::CollisionGroup* projectileCollisionGroup);
		virtual ~BasicTower();

		//! Updates the projectile.
		void update(sf::Time const &elapsedTime);

		//! Draws the tower and its projectile.
		virtual void draw(sf::RenderTarget &target) override;

		/*!
		\brief Tries to fire and the members of possibleTargets.
		Fires if projectile is not already in the air and a target is within range (Target must also be alive and part of ENEMY faction).
		\returns True if a target was chosen and fired upon.
		*/
		bool acquireTarget(std::list<Pawn*> const &possibleTargets);	//yet another method to be replaced by chaiscript

		//! Returns the money cost of placing this tower.
		static int getCost();
	};
}
#endif