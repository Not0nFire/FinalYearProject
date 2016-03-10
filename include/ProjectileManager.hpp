#ifndef PROJECTILE_MANAGER_HPP
#define PROJECTILE_MANAGER_HPP

#include <memory>
#include <list>
#include <SFML/Graphics.hpp>

class Projectile;
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
	*/
	ProjectileManager(std::shared_ptr<collision::CollisionGroup> projectileCollisionGroup);
	~ProjectileManager();

	/*!
	\brief Give a projectile to the manager.
	\param projectilePtr Move referenced unique pointer to a projectile.
	\remarks The manager gains ownership of the projectile.
	*/
	void give(unique_ptr<Projectile>&& projectilePtr);

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
	std::list<unique_ptr<Projectile>> mProjectiles;

	//! Group that projectiles will be checked against upon impact.
	std::shared_ptr<collision::CollisionGroup> mCollisionGroup;
};
#endif