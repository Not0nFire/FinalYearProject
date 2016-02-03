#include <include/Towers/UnitTower.hpp>

namespace tower {

	UnitTower::UnitTower(sf::Texture &texture, sf::Vector2f const& position, Path const& path, std::string const& xmlDefPath, std::function<void(Minion*)> spawnCallback) :
		Actor(texture,
		[]()
	{
		sf::ConvexShape* mask = new sf::ConvexShape(4u);
		mask->setPoint(0u, sf::Vector2f(0.f, -25.f));
		mask->setPoint(1u, sf::Vector2f(-55.f, 0.f));
		mask->setPoint(2u, sf::Vector2f(0.f, 25.f));
		mask->setPoint(3u, sf::Vector2f(55.f, 0.f));
		return mask;
	}(),

		sf::Vector2f(0.0f, 3.0f)),
		M_TICKS_PER_SPAWN(5000),
		mTicks(M_TICKS_PER_SPAWN),
		mUnitDefPath(xmlDefPath),
		mSpawnCallback(spawnCallback)
	{
		auto bounds = getLocalBounds();
		setOrigin(bounds.width * .5f, bounds.height * 0.85f);
		setPosition(position);
		updateCollidableMask(getPosition());

		//find nearest path node to deploy units to
		mNearestPathNode = path.begin();
		float distToNearest = thor::length(getPosition() - mNearestPathNode->getCentre());
		for (Node* node = mNearestPathNode->getNext();
			node != nullptr;
			node = node->getNext())
		{
			float distToNode = thor::length(getPosition() - node->getCentre());
			if (distToNode < distToNearest)
			{
				mNearestPathNode = node;
				distToNearest = distToNode;
			}
		}
	}

	UnitTower::~UnitTower() {
	}

	void UnitTower::update(sf::Time const& elapsedTime) {
		mTicks = ++mTicks % UINT32_MAX;
		if (mTicks >= M_TICKS_PER_SPAWN)
		{
			mTicks -= M_TICKS_PER_SPAWN;
			mSpawnCallback(spawnUnit());
		}
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
