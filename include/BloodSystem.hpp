#ifndef BLOOD_SYSTEM_HPP
#define BLOOD_SYSTEM_HPP

#include <Thor/Particles.hpp>
#include <Thor/Math.hpp>
#include <SFML/Graphics.hpp>

class BloodSystem : public sf::Drawable {
public:
	explicit BloodSystem(sf::Texture &texture);
	~BloodSystem();

	void addSpurt(sf::Vector2f const& position);
	void addSpurt(sf::Vector2f const& position, sf::Vector2f const& direction, float speed = 50.f, float deflectionAngle = 30.f);

	void update(sf::Time const& time);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	thor::ParticleSystem mParticleSystem;

	static const thor::Distribution<sf::Vector2f> mRandomDirection;
};
#endif