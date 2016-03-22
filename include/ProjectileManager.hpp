#ifndef PROJECTILE_MANAGER_HPP
#define PROJECTILE_MANAGER_HPP

#include <memory>
#include <list>
#include <SFML/Graphics.hpp>
#include <Thor/Particles.hpp>

class Projectile;
class FancyProjectile;
namespace collision { class CollisionGroup; }

using std::unique_ptr;

/*!
\class ProjectileManager
\brief Updates a collection of projectiles.
Projectiles are stored in unique_ptrs.
*/
class ProjectileManager {
public:
	/*!
	\brief Constructs an instance of ProjectileManager.
	\param projectileCollisionGroup The collision group against which projectiles will be checked upon impact.
	\param particleTexture Texture used by particles emiited from FancyProjectiles.
	*/
	ProjectileManager(std::shared_ptr<collision::CollisionGroup> projectileCollisionGroup, sf::Texture const& particleTexture);
	~ProjectileManager();

	/*!
	\brief Give a projectile to the manager.
	\param projectilePtr Move referenced unique pointer to a projectile.
	\remarks The manager gains ownership of the projectile.
	*/
	void give(std::shared_ptr<Projectile> const &projectilePtr);

	/*!
	\brief Calls update on all managed projectiles.
	*/
	void update(sf::Time const& elapsedTime);

	/*!
	\brief Draws all the managed projectiles.
	*/
	void draw(sf::RenderTarget &w);

private:
	//! The collection of projectiles to be managed.
	std::list<std::shared_ptr<Projectile>> mProjectiles;

	//! Group that projectiles will be checked against upon impact.
	std::shared_ptr<collision::CollisionGroup> mCollisionGroup;

	thor::ParticleSystem mParticleSystem;
};
#include "include/Projectile.h"
#include "include/FancyProjectile.hpp"
#include "include/Collision/CollisionGroup.hpp"
#endif