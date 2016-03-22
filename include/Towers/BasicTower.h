#ifndef _BASIC_TOWER_H
#define _BASIC_TOWER_H

#include <include/Actor.hpp>
#include <include/Damage.hpp>
#include <include/Pawn.hpp>
#include <include/Collision/CollisionGroup.hpp>
#include <include/ResourceManager.hpp>
#include "include/ProjectileManager.hpp"
#include <include/ArcProjectile.h>

namespace tower
{
	/*!
	\class BasicTower
	\brief Tower that fires a projectile toward a target.
	Takes account for moving targets.
	*/
	class BasicTower : public Actor {
	public:
		/*!
		\brief Consructs a Basic Tower.
		\param texture Visual representation of the tower.
		\param position Position of the tower in the Level.
		\param range Radius around the tower, within which it will fire upon units.
		\param attacksPerSecond How many projectiles can be fired every second.
		\param damage Amount of damage each projectile inflicts.
		\param damageType The damageType of each projectile.
		\param projectileMgr Shared ptr to a ProjectileManager, into which we will pass any projectiles we fire.
		*/
		BasicTower(
			sf::Texture &texture,
			sf::Vector2f const &position,
			float range,
			float attacksPerSecond,
			int damage,
			Damage::Type damageType,
			std::shared_ptr<ProjectileManager> projectileMgr
			);
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
		virtual bool acquireTarget(std::list<Pawn*> const &possibleTargets);

		//! Returns the money cost of placing this tower.
		static int getCost();

		//Returns a copy of the placement mask.
		static sf::ConvexShape getMask();
		
	protected:
		//! Radius that the tower can fire within.
		float mRange;

		float mSecondsPerAttack;

		float mSecondsSinceLastAttack;

		//! The type of damage to inflick (PHYSICAL or MAGICAL)
		Damage::Type mDamageType;

		//! The amount of damage each projectile inflicts.
		int mDamage;

		//! Relative position at which to fire the projectile from.
		sf::Vector2f mProjectileSpawnOffset;

		/*!
		\brief Predicts where a marching Pawn will be after a certain number of seconds.
		\returns A vector of where the Pawn will be if it keeps moving in its current direction.
		*/
		sf::Vector2f leadTarget(Pawn* target, float time) const;

		//! How much money the tower costs to place.
		static int mCost;

		//! Shape used for placement and collision.
		static sf::ConvexShape mPlacementMask;

		std::shared_ptr<ProjectileManager> mProjectileManager;
	};
}
#endif