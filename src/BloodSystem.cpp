#include <include/BloodSystem.hpp>

using namespace thor::Distributions;

const thor::Distribution<sf::Vector2f> BloodSystem::mRandomDirection = deflect({ 1.f, 0.f }, 360.f);

BloodSystem::BloodSystem(sf::Texture& texture) : mParticleSystem() {
	mParticleSystem.setTexture(texture);

	//Ensure that particles fall under gravity
	mParticleSystem.addAffector(
		[](thor::Particle& particle, sf::Time time)
	{
		static const auto gravity = 100.f;
		particle.velocity.y += gravity * time.asSeconds();
	});
}

BloodSystem::~BloodSystem() {}

void BloodSystem::addSpurt(sf::Vector2f const& position) {
	addSpurt(position, mRandomDirection());
}

void BloodSystem::addSpurt(sf::Vector2f const& position, sf::Vector2f const& direction, float speed, float deflectionAngle) {
	thor::UniversalEmitter emitter;

	emitter.setParticleLifetime(uniform(sf::seconds(0.1f), sf::seconds(1.f)));
	emitter.setParticlePosition(position);
	emitter.setEmissionRate(100.f);
	emitter.setParticleVelocity(deflect(direction * speed, deflectionAngle));
	
	mParticleSystem.addEmitter(emitter, sf::seconds(1.f));
}

void BloodSystem::update(sf::Time const& time) {
	mParticleSystem.update(time);
}

void BloodSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mParticleSystem, states);
}
