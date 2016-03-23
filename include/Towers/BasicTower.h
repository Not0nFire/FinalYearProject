#ifndef _BASIC_TOWER_H
#define _BASIC_TOWER_H

#include "Tower.h"
#include <include/Damage.hpp>
#include <include/Pawn.hpp>
#include <include/Collision/CollisionGroup.hpp>
#include <include/ResourceManager.hpp>
#include "include/ProjectileManager.hpp"
#include <include/ArcProjectile.h>

namespace tower
{
	/*!
	\class ProjectileTower
	\brief Tower that fires a projectile toward a target.
	Fires in an arc and takes account for moving targets.
	*/
	class ProjectileTower : public Tower {
	public:
		/*!
		\brief Consructs a Projectile Tower.
		\param position Position to create the tower at.
		\param xmlDef Xml element containing tower definition.
		*/
		ProjectileTower(sf::Vector2f const &position, tinyxml2::XMLElement *xmlDef);
		virtual ~ProjectileTower();

		/*!
		\brief Tries to fire at the members of possibleTargets.
		\returns True if a target was chosen and fired upon.
		*/
		virtual bool shoot(std::shared_ptr<std::list<std::shared_ptr<Pawn>>> const &possibleTargets) override;

		void setProjectileManager(std::shared_ptr<ProjectileManager> projectileMgr);
		
	protected:
		//! Radius that the tower can fire within.
		float mRange;

		//! The type of damage to inflict (PHYSICAL or MAGICAL)
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

		std::shared_ptr<ProjectileManager> mProjectileManager;
	};
}
#endif