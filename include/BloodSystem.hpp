#ifndef BLOOD_SYSTEM_HPP
#define BLOOD_SYSTEM_HPP

#include <Thor/Particles.hpp>
#include <Thor/Math.hpp>
#include <SFML/Graphics.hpp>
#include <Thor/Animations/FadeAnimation.hpp>

class BloodSystem : public sf::Drawable {
public:
	explicit BloodSystem(sf::Texture &texture, std::function<void(Drawable const&)> underlayDrawCallback);
	~BloodSystem();

	void addSpurt(sf::Vector2f const& position);
	void addSpurt(sf::Vector2f const& position, sf::Vector2f const& direction, float speed = 40.f, float deflectionAngle = 30.f, float minTtl = 0.8f, float maxTtl = 1.2f);

	void update(sf::Time const& time);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	thor::ParticleSystem mSpurtSystem;

	static const thor::Distribution<sf::Vector2f> mRandomDirection;

	std::function<void(Drawable const&)> mPersistDraw;
	sf::Sprite mPersistantParticle;
	void persistParticle(thor::Particle const& particle);
};
#endif