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

void ProjectileManager::give(unique_ptr<Projectile>&& projectilePtr) {
	//Set the onHit function of the projectile to check against the collision group
	projectilePtr->setOnHit(bind(&collision::CollisionGroup::checkAgainst, mCollisionGroup.get(), std::placeholders::_1));

	//If it's a fancy projectile, attach it to the particle system.
	FancyProjectile* fancy = dynamic_cast<FancyProjectile*>(projectilePtr.get());
	if (fancy) {
		fancy->attachToParticleSystem(mParticleSystem);
	}

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
			//make an explosion when a magic projectile is erased
			auto fancy = dynamic_cast<FancyProjectile*>(projectile.get());
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
	for (unique_ptr<Projectile>& projectile : mProjectiles)
	{
		projectile->draw(w);
		projectile->debug_draw(w);
	}
}
