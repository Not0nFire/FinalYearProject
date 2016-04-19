#include <include/BloodSystem.hpp>

using namespace thor::Distributions;

const thor::Distribution<sf::Vector2f> BloodSystem::mRandomDirection = deflect({ 0.f, -1.f }, 60.f);

inline int rangedRandom(int min, int max) {
	return rand() % (max + 1 - min) + min;
}

BloodSystem::BloodSystem(sf::Texture& texture, std::function<void(Drawable const&)> underlayDrawCallback) :
mSpurtSystem(),
mPersistantParticle(texture),
mPersistDraw(underlayDrawCallback)
{
	mSpurtSystem.setTexture(texture);

	//Ensure that particles fall under gravity
	mSpurtSystem.addAffector(
		[this](thor::Particle& particle, sf::Time time)
	{
		static const auto gravity = 80.f;
		static const sf::Time noTime = sf::seconds(0.f);
		particle.velocity.y += gravity * time.asSeconds();

		//If we think particle will be removed by next update...
		if (getRemainingLifetime(particle) - time < noTime || particle.velocity.y > rangedRandom(60, 140))
		{
			//...make the particle persist
			persistParticle(particle);
			//abondonit so we don't persist it more than once
			abandonParticle(particle);
		}
	});

	//mSpurtSystem.addAffector(thor::AnimationAffector(thor::FadeAnimation(0.f, 0.25f)));
}

BloodSystem::~BloodSystem() {}

void BloodSystem::addSpurt(sf::Vector2f const& position) {
	addSpurt(position, mRandomDirection());
}

void BloodSystem::addSpurt(sf::Vector2f const& position, sf::Vector2f const& direction, float speed, float deflectionAngle, float minTtl, float maxTtl) {
	thor::UniversalEmitter emitter;

	emitter.setParticleLifetime(uniform(sf::seconds(5), sf::seconds(10)));
	emitter.setParticlePosition(position);
	emitter.setEmissionRate(500.f);
	emitter.setParticleVelocity(deflect(direction * speed, deflectionAngle));
	
	mSpurtSystem.addEmitter(emitter, uniform(sf::seconds(0.1f), sf::seconds(0.8f))());
}

void BloodSystem::update(sf::Time const& time) {
	mSpurtSystem.update(time);
}

void BloodSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mSpurtSystem, states);
}

void BloodSystem::persistParticle(thor::Particle const& particle) {
	mPersistantParticle.setPosition(particle.position);
	mPersistantParticle.setColor(particle.color);
	
	mPersistDraw(mPersistantParticle);
}
