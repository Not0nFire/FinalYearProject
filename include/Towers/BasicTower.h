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
	Tower that fires a projectile toward a target.
	Aim does not currently take account for moving targets.
	*/
	class BasicTower : public Actor {
		
		float mRange;
		float mAttacksPerSecond;

		Damage::Type mDamageType;
		int mDamage;

		Projectile mProjectile;

		sf::Vector2f leadTarget(Pawn* target, float time) const;

	public:
		BasicTower(sf::Texture &texture, sf::Vector2f position, float range, float attacksPerSecond, int damage, Damage::Type damageType, collision::CollisionGroup* projectileCollisionGroup);
		virtual ~BasicTower();

		void update(sf::Time const &elapsedTime);

		virtual void draw(sf::RenderTarget &target) override;

		bool acquireTarget(std::vector<Pawn*> const &possibleTargets);	//yet another method to be replaced by chaiscript
	};
}
#endif