#include <include/Abilities/RaiseDead.hpp>

using namespace abilities;

RaiseDead::RaiseDead(tinyxml2::XMLElement* xml) :
Ability(xml),
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

void RaiseDead::execute(Pawn* user) {
	const auto& userPosition = user->getPosition();

	//Just stop the user from moving momentarily.
	user->setDestination(userPosition);

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

void RaiseDead::doUpdateLogic(float deltaSeconds) {
	deactivate();
}

void RaiseDead::draw(sf::RenderTarget& target, sf::RenderStates states) const {}
