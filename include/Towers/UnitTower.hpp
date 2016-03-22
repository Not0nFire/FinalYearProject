#ifndef _UNIT_TOWER_H
#define _UNIT_TOWER_H

#include "Tower.h"
#include <include/Pathing/Path.hpp>
#include <include/Minion.hpp>
#include <include/TinyXML2/tinyxml2.h>

namespace tower {

	/*!
	\brief Tower that produces Minions and sends them to the nearest path node.
	Uses an xml Minion definition to create units.
	*/
	class UnitTower : public Tower {
	public:
		/*!
		\brief Constructs a unit-producing tower.
		\param position Position of the tower in the level.
		\param xmlDef Xml element containing tower definition
		*/
		UnitTower(sf::Vector2f const &position, tinyxml2::XMLElement *xmlDef);
		virtual ~UnitTower();

		/*!
		\brief Spawns a unit if enough time has passed.
		*/
		void update(sf::Time const &elapsedTime) override;

		bool shoot(std::list<std::shared_ptr<Pawn>> const& targetList) override;

		void setPath(Path const& path);

		void setSpawnCallback(std::function<void(Minion*)> const &callback);

	protected:
		//! Path to xml file containing Minion definition.
		std::string mUnitDefPath;

		//! Function to be called when a unit is spawned. Used for adding units to list in Level.
		std::function<void(Minion*)> mSpawnCallback;

		std::vector<Minion*> mSpawnedUnits;

		/*!
		\brief Creates a new Minion from xml definition.
		\returns A pointer to a newly created Minion.
		*/
		virtual Minion* spawnUnit();

		//! Pointer to nearest path node. This is where spawned units are sent.
		Node* mNearestPathNode;
	};
}
#endif