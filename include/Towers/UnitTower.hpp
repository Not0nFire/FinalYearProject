#ifndef _UNIT_TOWER_H
#define _UNIT_TOWER_H

#include <include/Actor.hpp>
#include <include/Pathing/Path.hpp>
#include <include/Minion.hpp>
#include <include/TinyXML2/tinyxml2.h>

namespace tower {

	/*!
	\brief Tower that produces Minions and sends them to the nearest path node.
	Uses an xml Minion definition to create units.
	*/
	class UnitTower : public Actor {
	protected:
		//! Path to xml file containing Minion definition.
		std::string mUnitDefPath;

		//! Function to be called when a unit is spawned. Used for adding units to list in Level.
		std::function<void(Minion*)> mSpawnCallback;

		//! How many ticks must pass before a unit is spawned.
		const unsigned int M_TICKS_PER_SPAWN;

		//! A counter for ticks. Incremented every update() and subtracted from when a unit is spawned.
		unsigned int mTicks;

		/*!
		\brief Creates a new Minion from xml definition.
		\returns A pointer to a newly created Minion.
		*/
		virtual Minion* spawnUnit();

		//! Pointer to nearest past node. This is where spawned units are sent.
		Node* mNearestPathNode;

	public:
		/*!
		\brief Constructs a UnitTower.
		\param texture Visual representation of the tower.
		\param position Position of the tower in the Level.
		\param path Path that units will be sent to. The contructor will pick the closest node.
		\param xmlDefPath Path to xml file containing Minion definition. Used to create units.
		\param spawnCallback Invoked whenever a Minion is created. Intended use is for adding to list of Pawns in Level.
		*/
		UnitTower(sf::Texture &texture, sf::Vector2f const &position, Path const &path, std::string const &xmlDefPath, std::function<void(Minion*)> spawnCallback);
		virtual ~UnitTower();

		/*!
		\brief Spawns a unit if enough time has passed.
		Increments tick counter; If tick counter is greater than or equal to M_TICKS_PER_SPAWN,
		spawnUnit() is called and M_TICKS_PER_SPAWN is subtracted from the counter.
		*/
		virtual void update(sf::Time const &elapsedTime);
	};
}
#endif