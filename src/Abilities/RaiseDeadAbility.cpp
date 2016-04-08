#include <include/Abilities/RaiseDeadAbility.hpp>

using namespace abilities;

RaiseDead::RaiseDead(tinyxml2::XMLElement* xml) :
Ability(xml),
mCastGraphics(xml->FirstChildElement("Actor")),
mUnitPrototype(xml->FirstChildElement("Minion"))
{
	auto spawnDistance = atoi(xml->FirstChildElement("SpawnDistance")->GetText());
	auto unitsToSpawn = atoi(xml->FirstChildElement("BaseNumberUnits")->GetText());
	mSpawnPoints.reserve(unitsToSpawn);

	//Integer division intentional.
	auto deltaAngle = 360 / unitsToSpawn;

	//Calculate spawn points evenly in a circle
	for (auto numSpawnPoints = 0; numSpawnPoints < unitsToSpawn; ++numSpawnPoints) {
		auto angle = numSpawnPoints * deltaAngle;

		mSpawnPoints.emplace_back(
			spawnDistance * cosf(angle % 360),	// X value
			spawnDistance * sinf(angle % 360)	// Y value
			);
	}
}

RaiseDead::~RaiseDead() {}

void RaiseDead::doExecuteLogic(Pawn* user) {
	const auto& userPosition = user->getPosition();

	//Set the casting graphics to the user's position and lay the casting animation.
	mCastGraphics.setPosition(user->getPosition());
	mCastGraphics.playAnimation("Cast");

	//Spawn units around the user.
	for (auto const& spawnPoint : mSpawnPoints) {
		auto unit = std::make_shared<Minion>(mUnitPrototype);
		unit->playAnimation("Spawn");

		auto actualSpawnPosition = userPosition + spawnPoint;
		unit->setPosition(actualSpawnPosition);
		unit->setDestination(actualSpawnPosition);

		spawnPawn(unit);
	}
}

void RaiseDead::doUpdateLogic(sf::Time const& deltaTime) {
	//Deactivaate when mCastGraphics finishes its animation.
	if (mCastGraphics.isPlayingAnimation()) {
		mCastGraphics.animate(deltaTime);
	}
	else {
		deactivate();
	}
}

void RaiseDead::draw(sf::RenderTarget& target, sf::RenderStates states) const {}
