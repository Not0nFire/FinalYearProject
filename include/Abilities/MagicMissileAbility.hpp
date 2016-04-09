#ifndef MAGIC_MISSILE_ABILITY_HPP
#define MAGIC_MISSILE_ABILITY_HPP

#include "Ability.hpp"
#include <include/Damage.hpp>

namespace abilities {
	/*!
	\class MagicMissile
	\brief Launches homing projectiles at enemies.
	*/
	class MagicMisile : public Ability {
	public:
		explicit MagicMisile(tinyxml2::XMLElement* xml);
		virtual ~MagicMisile();

	protected:
		/*!
		\brief Launches homing projectiles that target enemies.
		\param The user of this ability. Ownership is not gained and the pointer is not stored.
		*/
		void doExecuteLogic(Pawn* user) override;

		/*!
		\brief Animates the ability graphics and spawns missiles.
		The missiles are spawned throughout the cast time and are given random initial velocities.
		\param deltaSeconds The time since the last update loop iteration.
		*/
		void doUpdateLogic(sf::Time const& deltaTime) override;

		/*!
		\brief Draws the ability graphics.
		*/
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	private:
		//! The number of missiles to spawn.
		const unsigned int M_NUM_MISSILES;

		//! The type of damage the missiles will deal.
		Damage::Type mDamageType;

		//! The amount of damage each missile will deal.
		const float M_MISSILE_DAMAGE;

		//! How many missiles have been spawned so far since execute().
		unsigned int mMissilesSpawned;

		//Time between missile spawns.
		float mMissileSpawnInterval;
		//Time since a missile was last since.
		float mTimeSinceMissileSpawn;

		Actor mCastGraphics;
		sf::Sound mLaunchSound;
	};
}
#endif