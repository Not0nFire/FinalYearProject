#include <include/Towers/UnitTower.hpp>

namespace tower {

	UnitTower::UnitTower(sf::Vector2f const &position, tinyxml2::XMLElement *xmlDef) :
		Tower(position, xmlDef),
		mUnitDefPath(xmlDef->FirstChildElement("UnitDefinition")->Attribute("path"))
	{
		const int maxUnits = atoi(xmlDef->FirstChildElement("MaxUnits")->GetText());

		//Reserve and fill the required number of places in the vector
		mSpawnedUnits.reserve(maxUnits);
		for (unsigned i = 0; i < maxUnits; ++i) {
			mSpawnedUnits.push_back(nullptr);
		}
	}

	UnitTower::~UnitTower() {
	}

	void UnitTower::update(sf::Time const& elapsedTime) {
		Tower::update(elapsedTime);

		//If we have a container to put units into AND we have a path node to send them to AND it's time to spawn a unit...
		if (mSpawnCallback && !mNearestPathNode.expired() && mSecondsSinceLastAttack >= mSecondsPerAttack)
		{
			//Even if we can't spawn a unit, we'll have to wait our turn to try again.
			mSecondsSinceLastAttack = 0.f;

			auto size = mSpawnedUnits.size();
			for (auto i = 0; i < size; ++i) {
				auto unit = mSpawnedUnits[i];

				//If the minion is null or dead...
				if (nullptr == unit || unit->isDead()) {
					//...spawn a new one.
					unit = spawnUnit();
					mSpawnedUnits[i] = unit;
					mSpawnCallback(unit);
					break;
				}//end if(null||dead)
			}//end for
		}//end if
	}

	bool UnitTower::shoot(std::shared_ptr<std::list<std::shared_ptr<Pawn>>> const& targetList) {
		return false;
	}

	void UnitTower::setPath(std::shared_ptr<Path> const &path) {
		using thor::length;

		auto nearestNode = path->begin();
		auto node = nearestNode->getNext();

		const auto &myPosition = getPosition();
		auto distanceToNearestNode = length(nearestNode->getCentre() - myPosition);

		for (; nullptr != node; node = node->getNext()) {
			//If the distance to node is closer than distance to mNearestPathNode...
			if (distanceToNearestNode > length(node->getCentre() - myPosition)) {
				//...update the nearest node.
				nearestNode = node;
				distanceToNearestNode = length(nearestNode->getCentre() - myPosition);
			}//end if
		}//end while

		mNearestPathNode = nearestNode;
	}

	void UnitTower::setSpawnCallback(std::function<void(std::shared_ptr<Minion>)> const& callback) {
		mSpawnCallback = callback;
	}

	std::shared_ptr<Minion> UnitTower::spawnUnit() {
		tinyxml2::XMLDocument doc;

		tinyxml2::XMLError result = doc.LoadFile(mUnitDefPath.c_str());
		if (result != tinyxml2::XML_NO_ERROR)
		{
			throw result;
		}

		auto unit = std::make_shared<Minion>(doc.FirstChildElement("Minion"));
		unit->setPosition(getPosition());

		if (auto node = mNearestPathNode.lock()) {
			unit->setDestination(node->getPoint());
		}

		return unit;
	}
}
