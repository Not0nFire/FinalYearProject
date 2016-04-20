#include <include/Abilities/MagicMissileAbility.hpp>
#include <include/Settings.hpp>

using namespace abilities;

#define GET_XML_VALUE(elementName) xml->FirstChildElement(elementName)->GetText()
MagicMisile::MagicMisile(tinyxml2::XMLElement* xml) :
Ability(xml),
M_NUM_MISSILES(atoi(GET_XML_VALUE("BaseNumberOfMissiles"))),
M_MISSILE_DAMAGE(atoi(GET_XML_VALUE("BaseDamagePerMissile"))),
mCastGraphics(xml->FirstChildElement("Actor"))
{
	std::string dmgType = GET_XML_VALUE("DamageType");

	if (dmgType == "PHYSICAL")
	{
		mDamageType = Damage::PHYSICAL;
	}
	else //if (dmgType == "MAGICAL")
	{
		mDamageType = Damage::MAGICAL;
	}

	std::string launchSoundPath = xml->FirstChildElement("MissileLaunchSound")->Attribute("path");
	auto &buffer = ResourceManager<sf::SoundBuffer>::instance()->get(launchSoundPath);
	mLaunchSound.setBuffer(buffer);
	mLaunchSound.setVolume(Settings::getInt("EffectsVolume"));
}

MagicMisile::~MagicMisile() {
}

void MagicMisile::doExecuteLogic(Pawn* user) {
	//Set the casting graphics to the user's position and play the casting animation.
	mCastGraphics.setPosition(user->getPosition());
	mCastGraphics.playAnimation("Cast");

	mMissilesSpawned = 0u;
	mMissileSpawnInterval = getCastDuration().asSeconds() / M_NUM_MISSILES;
	mTimeSinceMissileSpawn = mMissileSpawnInterval;

	mLaunchSound.setPosition(sf::Vector3f(user->getPosition().x, user->getPosition().y, 0.f));
}

void MagicMisile::doUpdateLogic(sf::Time const& deltaTime) {
	mCastGraphics.animate(deltaTime);

	float deltaSeconds = deltaTime.asSeconds();
	mTimeSinceMissileSpawn += deltaSeconds;

	if (mTimeSinceMissileSpawn >= mMissileSpawnInterval)
	{
		mTimeSinceMissileSpawn = 0.f;

		//We don't set the target or fire the projectile here, so the ProjectileManager will fire it when it receives the projectile.
		auto missile = (std::make_shared<FancyProjectile>(
			M_MISSILE_DAMAGE,
			mDamageType,
			ResourceManager<sf::Texture>::instance()->get("./res/img/magic_missile.png"),
			25
			));

		missile->setPosition(mCastGraphics.getPosition());

		spawnProjectile(missile);
		mLaunchSound.play();

		if (++mMissilesSpawned == M_NUM_MISSILES)
		{
			//Deactivate the ability if we have spawned the correct number of missiles.
			deactivate();
		}
	}
}

void MagicMisile::doDraw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (isActive()) {
		target.draw(mCastGraphics, states);
	}
}