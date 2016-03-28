#include <include/Abilities/MagicMissileAbility.hpp>

using namespace abilities;

#define GET_XML_VALUE(elementName) xml->FirstChildElement(elementName)->GetText()
MagicMisile::MagicMisile(tinyxml2::XMLElement* xml) :
Ability(xml),
mCastGraphics(xml->FirstChildElement("Actor")),
M_CAST_DURATION(atof(GET_XML_VALUE("CastDuration"))),
M_NUM_MISSILES(atoi(GET_XML_VALUE("BaseNumberOfMissiles"))),
M_MISSILE_DAMAGE(atoi(GET_XML_VALUE("BaseDamagePerMissile")))
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
}

MagicMisile::~MagicMisile() {
}

void MagicMisile::execute(Pawn* user) {
	//Activate the ability, without doing this, the ability won't be updated or drawn.
	activate();

	//Stun the user for the duration of the cast time.
	user->stun(sf::seconds(M_CAST_DURATION));

	//Set the casting graphics to the user's position and lay the casting animation.
	mCastGraphics.setPosition(user->getPosition());
	mCastGraphics.playAnimation("Cast");

	mMissilesSpawned = 0u;
	mMissileSpawnInterval = M_CAST_DURATION / M_NUM_MISSILES;
	mTimeSinceMissileSpawn = mMissileSpawnInterval;
}

void MagicMisile::doUpdateLogic(float deltaSeconds) {
	mCastGraphics.animate(sf::seconds(deltaSeconds));
	mTimeSinceMissileSpawn += deltaSeconds;

	if (mTimeSinceMissileSpawn >= mMissileSpawnInterval)
	{
		mTimeSinceMissileSpawn = 0.f;

		//We don't set the target or fire the projectile here, so the ProjectileManager will fire it when it receives the projectile.
		spawnProjectile(std::make_shared<FancyProjectile>(
			M_MISSILE_DAMAGE,
			mDamageType,
			ResourceManager<sf::Texture>::instance()->get("./res/img/magic_projectile.png")
			));
	}

	if (mMissilesSpawned == M_NUM_MISSILES)
	{
		//Deactivate the ability if we have spawned the correct number of missiles.
		deactivate();
	}
}

void MagicMisile::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mCastGraphics, states);
}