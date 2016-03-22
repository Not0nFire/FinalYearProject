#include <include/Towers/UnitTower.hpp>

namespace tower {

	UnitTower::UnitTower(sf::Vector2f const &position, tinyxml2::XMLElement *xmlDef) :
		Tower(position, xmlDef),
		mUnitDefPath(xmlDef->FirstChildElement("UnitDefinition")->Attribute("path")),
		mNearestPathNode(nullptr)
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

		if (mSecondsSinceLastAttack >= mSecondsPerAttack)
		{
			//Even if we can't spawn a unit, we'll have to wait our turn to try again.
			mSecondsSinceLastAttack = 0.f;

			auto size = mSpawnedUnits.size();
			for (auto i = 0; i < size; ++i) {
				Minion* unit = mSpawnedUnits[i];

				//If the minion is null or dead...
				if (nullptr == unit || unit->isDead()) {
					//...spawn a new one.
					//(We can just forget about the old minion, we don't have ownership of it.
					// We'll also lose ownership of the newly spawned minion when we pass it to mSpawnCallback())
					unit = spawnUnit();
					mSpawnedUnits[i] = unit;
					mSpawnCallback(unit);
					break;
				}//end if(null||dead)
			}//end for
		}//end if
	}

	bool UnitTower::shoot(std::list<std::shared_ptr<Pawn>> const& targetList) {
		return false;
	}

	void UnitTower::setPath(Path const& path) {
		using thor::length;

		mNearestPathNode = path.begin();
		auto node = mNearestPathNode->getNext();

		const auto &myPosition = getPosition();
		auto distanceToNearestNode = length(mNearestPathNode->getCentre() - myPosition);

		while(nullptr != node) {
			//If the distance to node is closer than distance to mNearestPathNode...
			if (distanceToNearestNode > length(node->getCentre() - myPosition)) {
				//...update the nearest node.
				mNearestPathNode = node;
				distanceToNearestNode = length(mNearestPathNode->getCentre() - myPosition);
			}//end if
		}//end while
	}

	void UnitTower::setSpawnCallback(std::function<void(Minion*)> const &callback) {
		mSpawnCallback = callback;
	}

	Minion* UnitTower::spawnUnit() {
		tinyxml2::XMLDocument doc;

		tinyxml2::XMLError result = doc.LoadFile(mUnitDefPath.c_str());
		if (result != tinyxml2::XML_NO_ERROR)
		{
			throw result;
		}

		Minion* unit = new Minion(doc.FirstChildElement("Minion"));
		unit->setPosition(getPosition());
		unit->setDestination(mNearestPathNode->getPoint());

		return unit;
	}
}
