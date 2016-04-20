#include <include/Hero.hpp>
#include <include/Constants.h>
#include <include/Settings.hpp>

Hero::Hero(tinyxml2::XMLElement* xml) :
Pawn(xml->FirstChildElement("Pawn")),
mSecondsSinceRegen(0.f),
M_REGEN_INTERVAL(1.f / xml->FirstChildElement("HealthRegen")->FloatAttribute("rate")),
M_HEALTH_REGEN(atoi(xml->FirstChildElement("HealthRegen")->GetText()))
{
	_ASSERT(std::string(xml->Name()) == "Hero");

	playAnimation("idle", true);

	auto moveSfxXml = xml->FirstChildElement("MoveSounds");
	mMoveSounds.reserve(moveSfxXml->IntAttribute("amount"));

	//load movement confirmation sounds
	for (moveSfxXml = moveSfxXml->FirstChildElement("Sound");
		nullptr != moveSfxXml;
		moveSfxXml = moveSfxXml->NextSiblingElement("Sound"))
	{
		mMoveSounds.emplace_back(ResourceManager<sf::SoundBuffer>::instance()->get(moveSfxXml->Attribute("path")));
		mMoveSounds.rbegin()->setVolume(Settings::getInt("EffectsVolume"));
	}
}

Hero::~Hero() {
}

void Hero::update(sf::Time const& elapsedTime) {
	mSecondsSinceRegen += elapsedTime.asSeconds();
	if (mState != DEAD && mSecondsSinceRegen >= M_REGEN_INTERVAL)
	{
		mSecondsSinceRegen -= M_REGEN_INTERVAL;
		heal(M_HEALTH_REGEN);
	}

	Pawn::update(elapsedTime);

	turnToFaceDirection(mDestination);

}

void Hero::setDestination(sf::Vector2f const& destination) {
	if (shouldPlayMoveSound()) {
		playMoveSound();
	}

	Pawn::setDestination(destination);
}

void Hero::doAttack(float secondsElapsed) {
	//allow the player to attack while moving
	Pawn::doAttack(secondsElapsed);
	doMarch(getDestination() - getPosition(), secondsElapsed);
}

bool Hero::shouldPlayMoveSound() {
	static auto chance = Constants::Numbers::getHeroMoveSoundChance();
	return (rand() % 100) <= chance;
}

void Hero::playMoveSound() {
	//play a random sound
	auto index = rand() % mMoveSounds.size();
	mMoveSounds.at(index).play();
}
