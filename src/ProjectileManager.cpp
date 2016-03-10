#include "include/ProjectileManager.hpp"
#include "include/Projectile.h"
#include "include/Collision/CollisionGroup.hpp"

ProjectileManager::ProjectileManager(std::shared_ptr<collision::CollisionGroup> projectileCollisionGroup) :
mCollisionGroup(projectileCollisionGroup)
{
	//empty ctor body
}

ProjectileManager::~ProjectileManager() {
	//empty dtor body
}

void ProjectileManager::give(unique_ptr<Projectile>&& projectilePtr) {
	//Set the onHit function of the projectile to check against the collision group
	projectilePtr->setOnHit(bind(&collision::CollisionGroup::checkAgainst, mCollisionGroup.get(), std::placeholders::_1));

	//Move projectile pointer into collection
	mProjectiles.push_back(std::move(projectilePtr));
}

void ProjectileManager::update(sf::Time const& elapsedTime) {
	auto itr = mProjectiles.begin();
	auto end = mProjectiles.end();

	while (itr != end)
	{
		auto& projectile = *itr;

		//Update projectile if active
		if (projectile->isActive())
		{
			projectile->update(elapsedTime);
			++itr;
		}
		else
		{
			//destroy if not active
			itr = mProjectiles.erase(itr);
		}
	}
}

void ProjectileManager::draw(sf::RenderTarget& w) {
	//Draw all projectiles
	for (unique_ptr<Projectile>& projectile : mProjectiles)
	{
		projectile->draw(w);
	}
}
