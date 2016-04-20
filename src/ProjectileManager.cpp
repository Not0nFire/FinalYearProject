#include "include/ProjectileManager.hpp"

ProjectileManager::ProjectileManager(std::shared_ptr<collision::CollisionGroup> projectileCollisionGroup, sf::Texture const& particleTexture) :
mCollisionGroup(projectileCollisionGroup)
{
	mParticleSystem.setTexture(particleTexture);
	mParticleSystem.addAffector(thor::AnimationAffector(thor::FadeAnimation(0.f, 0.25f)));	//make particles fade out at end of life
}

ProjectileManager::~ProjectileManager() {
	//empty dtor body
}

void ProjectileManager::give(std::shared_ptr<Projectile> const &projectilePtr) {

	//If the projectile has not been fired and we have a handler for this circumstance...
	if (!projectilePtr->isActive() && mUnfiredProjectileHandler)
	{
		mUnfiredProjectileHandler(projectilePtr);
	}

	//If it's a fancy projectile, attach it to the particle system.
	auto fancy = std::dynamic_pointer_cast<FancyProjectile, Projectile>(projectilePtr);
	if (fancy) {
		fancy->attachToParticleSystem(mParticleSystem);
	}

	//Move projectile pointer into collection
	mProjectiles.push_back(projectilePtr);
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
			if (projectile->impactOccured()) {
				auto collidable = std::static_pointer_cast<collision::Collidable, Projectile>(projectile);
				mCollisionGroup->checkAgainst(collidable);
			}
			++itr;
		}
		else
		{
			//make an explosion when a magic projectile is erased
			auto fancy = std::dynamic_pointer_cast<FancyProjectile, Projectile>(projectile);
			if (nullptr != fancy) {
				thor::UniversalEmitter emitter;
				emitter.setEmissionRate(100.f);
				emitter.setParticleVelocity(thor::Distributions::deflect({ 100.f, 0.f }, 360.f));
				emitter.setParticleLifetime(thor::Distributions::uniform(sf::seconds(0.1f), sf::seconds(0.5f)));
				emitter.setParticleRotationSpeed(thor::Distributions::uniform(10.f, 100.f));
				emitter.setParticlePosition(fancy->getPosition());
				mParticleSystem.addEmitter(emitter, sf::seconds(0.1f));
			}

			//destroy if not active
			itr = mProjectiles.erase(itr);
		}
	}

	mParticleSystem.update(elapsedTime);
}

void ProjectileManager::draw(sf::RenderTarget& w) {
	w.draw(mParticleSystem);

	//Draw all projectiles
	for (auto& projectile : mProjectiles)
	{
		projectile->draw(w);
		//projectile->debug_draw(w);
	}
}

void ProjectileManager::setUnfiredProjectileHandler(std::function<void(std::shared_ptr<Projectile> const&)> const& handler) {
	mUnfiredProjectileHandler = handler;
}
