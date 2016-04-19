#ifndef WAVE_CONTROLLER_HPP
#define WAVE_CONTROLLER_HPP

#include <queue>
#include <SFML/Graphics.hpp>
#include <include/TinyXML2/tinyxml2.h>
#include "UnitFactory.hpp"

namespace detail
{
	/*!
	\struct Wave
	\brief Stores information about a single wave.
	\see WaveController
	*/
	struct Wave {
		//! Parses a <Wave> xml tag
		Wave(tinyxml2::XMLElement* xml);
		//! Name of file containing unit definiton (w/o extension)
		const std::string unitDefinition;
		//! Seconds after previous wave that this wave should be spawned
		const float spawnDelay;
		//! Number of units to be spawned
		const int numberUnits;
	};
}

/*!
\class WaveController
\brief Spawns units in waves.
\see detail::Wave
*/
class WaveController {
public:
	/*!
	\brief Constructs a WaveController
	\param xml A <WaveController> tag, containing one or more <Wave>s.
	\param unitPath Path that spawned units will follow. Units are spawned the the beginning.
	\param spawnUnitCallback Function invoked when a unit is spawned. The unit is passed as the arguement.
	*/
	WaveController(tinyxml2::XMLElement* xml, std::function<void(std::shared_ptr<Minion>)> const& spawnUnitCallback);
	~WaveController();

	void update(sf::Time const& elapsedTime);
	bool isEmpty() const;
private:
	bool isTimeToSpawnWave() const;
	void spawnWave();

	//! Seconds since the last wave was spawned.
	float mTimeSinceWave;

	//! Queue of waves to be spawned.
	std::queue<detail::Wave> mWaveQueue;

	//! Factory used to create units.
	UnitFactory mFactory;

	//! Function called when a unit is to be spawned.
	std::function<void(std::shared_ptr<Minion> const&)> mSpawnUnitCallback;
};
#endif