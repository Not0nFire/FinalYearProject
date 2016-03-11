#ifndef MAGE_TOWER_HPP
#define MAGE_TOWER_HPP

#include "BasicTower.h"

namespace tower {
	/*!
	\class MageTower
	\brief Tower that fires magical projectiles that home in on their target.
	*/
	class MageTower : public BasicTower {
	public:
		/*!
		\brief Constructs a Mage Tower.
		\param texture Visual representation of the tower.
		\param position Position of the tower in the Level.
		\param range Radius around the tower, within which it will fire upon units.
		\param attacksPerSecond How many projectiles can be fired every second.
		\param damage Amount of damage each projectile inflicts.
		\param projectileMgr Shared ptr to a ProjectileManager, into which we will pass any projectiles we fire.
		*/
		MageTower(
			sf::Texture &texture,
			sf::Vector2f const &position,
			float range,
			float attacksPerSecond,
			int damage,
			std::shared_ptr<ProjectileManager> projectileManager
			);
		virtual ~MageTower();

		bool acquireTarget(std::list<Pawn*> const& possibleTargets) override;
	protected:

	};
}
#endif