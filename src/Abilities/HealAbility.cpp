#include <include/Abilities/HealAbility.hpp>

using namespace abilities;

Heal::Heal(tinyxml2::XMLElement* xml) :
Ability(xml),
mCastGraphics(xml->FirstChildElement("Actor")),
mHealAmount(atoi(xml->FirstChildElement("HealAmount")->GetText())),
mParticleSystem()
{
	auto particleXml = xml->FirstChildElement("Particles");

	std::string texturePath = particleXml->FirstChildElement("Texture")->Attribute("path");
	mParticleSystem.setTexture(ResourceManager<sf::Texture>::instance()->get(texturePath));

	float ttlMin = atof(particleXml->FirstChildElement("TimeToLive")->Attribute("min"));
	float ttlMax = atof(particleXml->FirstChildElement("TimeToLive")->Attribute("max"));
	mEmitter.setParticleLifetime(thor::Distributions::uniform(sf::seconds(ttlMin), sf::seconds(ttlMax)));

	float particlesPerSecond = atof(particleXml->FirstChildElement("EmissionRate")->GetText());
	mEmitter.setEmissionRate(particlesPerSecond);


	auto velXml = particleXml->FirstChildElement("Velocity");

	sf::Vector2f velocity(atof(velXml->Attribute("x")), atof(velXml->Attribute("y")));
	float variance = atof(velXml->Attribute("variance"));

	mEmitter.setParticleVelocity(thor::Distributions::deflect(velocity, variance));


	auto scaleXml = particleXml->FirstChildElement("Scale");
	if (nullptr != scaleXml) {
		sf::Vector2f scale;
		scale.x = atof(scaleXml->Attribute("x"));
		scale.y = atof(scaleXml->Attribute("y"));

		mEmitter.setParticleScale(scale);
	}
}

Heal::~Heal() {}

void Heal::doExecuteLogic(Pawn* user) {
	//Set the casting graphics to the user's position and play the casting animation.
	mCastGraphics.setPosition(user->getPosition());
	mCastGraphics.playAnimation("Cast");
	mCastGraphics.animate(sf::seconds(0.f));	//prevent the whole spritesheet from drawing if draw is called before first update

	user->heal(mHealAmount);

	mEmitter.setParticlePosition(user->getPosition());
	mParticleSystem.addEmitter(mEmitter, getCastDuration());
}

void Heal::doUpdateLogic(sf::Time const& deltaTime) {
	if (mCastGraphics.isPlayingAnimation()) {
		mCastGraphics.animate(deltaTime);
		mParticleSystem.update(deltaTime);
	}
	else {

		deactivate();
		mParticleSystem.clearEmitters();
	}
}

void Heal::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (isActive()) {
		target.draw(mCastGraphics, states);
		target.draw(mParticleSystem);
	}
}
