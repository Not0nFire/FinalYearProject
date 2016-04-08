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

	int deltaAngle = (360 / unitsToSpawn) * 0.017;

	//Calculate spawn points evenly in a circle
	for (auto numSpawnPoints = 0; numSpawnPoints < unitsToSpawn; ++numSpawnPoints) {
		int angle = numSpawnPoints * deltaAngle;

		mSpawnPoints.emplace_back(
			spawnDistance * cosf(angle % 360),	// X value
			spawnDistance * sinf(angle % 360)	// Y value
			);
	}
}

RaiseDead::~RaiseDead() {}

void RaiseDead::doExecuteLogic(Pawn* user) {
	//Set the casting graphics to the user's position and lay the casting animation.
	mCastGraphics.setPosition(user->getPosition());
	mCastGraphics.playAnimation("Cast");
	//mCastGraphics.animate(sf::seconds(0.f));

	
}

void RaiseDead::doUpdateLogic(sf::Time const& deltaTime) {
	//Deactivaate when mCastGraphics finishes its animation.
	if (mCastGraphics.isPlayingAnimation()) {
		mCastGraphics.animate(deltaTime);
	}
	else {
		auto const& position = mCastGraphics.getPosition();
		//Spawn units around the user.
		for (auto const& spawnPoint : mSpawnPoints) {
			auto unit = std::make_shared<Minion>(mUnitPrototype);
			unit->playAnimation("Spawn");

			auto actualSpawnPosition = position + spawnPoint;
			unit->setPosition(actualSpawnPosition);
			unit->setDestination(actualSpawnPosition);

			spawnMinion(unit);
		}

		deactivate();
	}
}

void RaiseDead::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (isActive()) {
		mCastGraphics.draw(target);
	}
}
