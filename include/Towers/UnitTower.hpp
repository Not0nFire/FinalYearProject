#ifndef _UNIT_TOWER_H
#define _UNIT_TOWER_H

#include <include/Actor.hpp>
#include <include/Pathing/Path.hpp>
#include <include/Minion.hpp>
#include <include/TinyXML2/tinyxml2.h>

namespace tower {

	class UnitTower : public Actor {
	protected:
		std::string mUnitDefPath;
		std::function<void(Minion*)> mSpawnCallback;

		const unsigned int M_TICKS_PER_SPAWN;
		unsigned int mTicks;

		virtual Minion* spawnUnit();

		Node* mNearestPathNode;

	public:
		UnitTower(sf::Texture &texture, sf::Vector2f const &position, Path const &path, std::string const &xmlDefPath, std::function<void(Minion*)> spawnCallback);
		virtual ~UnitTower();

		virtual void update(sf::Time const &elapsedTime);

		void draw(sf::RenderWindow &w);
	};
}
#endif