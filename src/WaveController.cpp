#include <include/WaveController.hpp>

detail::Wave::Wave(tinyxml2::XMLElement* xml) :
unitDefinition(xml->FirstChildElement("UnitType")->GetText()),
spawnDelay(atof(xml->FirstChildElement("SpawnDelay")->GetText())),
numberUnits(atoi(xml->FirstChildElement("NumberOfUnits")->GetText()))
{
	//empty Wave ctor body
}


WaveController::WaveController(tinyxml2::XMLElement* xml, std::function<void(std::shared_ptr<Minion>)> const& spawnUnitCallback) :
mTimeSinceWave(0.f),
mWaveQueue(),
mFactory(),
mSpawnUnitCallback(spawnUnitCallback)
{
	//For each <Wave> element...
	for (auto wave = xml->FirstChildElement("Wave");
		nullptr != wave;
		wave = wave->NextSiblingElement("Wave"))
	{
		//...create a wave and add it to the queue
		mWaveQueue.emplace(wave);
	}
}

WaveController::~WaveController() {}

void WaveController::update(sf::Time const& elapsedTime) {
	mTimeSinceWave += elapsedTime.asSeconds();

	if (!mWaveQueue.empty() && isTimeToSpawnWave()) {
		spawnWave();
	}
}

bool WaveController::isEmpty() const {
	return mWaveQueue.empty();
}

bool WaveController::isTimeToSpawnWave() const {
	return mTimeSinceWave >= mWaveQueue.front().spawnDelay;
}

void WaveController::spawnWave() {
	detail::Wave const& wave = mWaveQueue.front();

	//Produce a unit until we reach the goal
	for (auto i = 0; i < wave.numberUnits; ++i) {
		mSpawnUnitCallback(std::static_pointer_cast<Minion, Pawn>(mFactory.produce(wave.unitDefinition)));
	}

	//Pop the wave off the queue
	mWaveQueue.pop();

	//Reset the time since a wave has spawned
	mTimeSinceWave = 0.f;
}
