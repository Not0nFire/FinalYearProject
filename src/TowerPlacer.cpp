#include <include/Towers/TowerPlacer.hpp>

const sf::Color TowerPlacer::mValidColor = sf::Color::Green;
const sf::Color TowerPlacer::mInvalidColor = sf::Color::Red;

const std::string TowerPlacer::mArrowTowerDefPath = "./res/xml/arrow_tower.def.xml";
const std::string TowerPlacer::mMagicTowerDefPath = "./res/xml/mage_tower.def.xml";
const std::string TowerPlacer::mUnitTowerDefPath = "./res/xml/unit_tower.def.xml";

TowerPlacer::TowerPlacer(shared_ptr<TerrainTree> const &terrainTree, shared_ptr<ProjectileManager> const &projectileMgr, shared_ptr<Path> const &path, shared_ptr<std::list<Minion*>> &flock) :
mIsActive(false),
mIsValid(false),
mTerrainTree(terrainTree),
mOverlay(ResourceManager<sf::Texture>::instance()->get("./res/img/tower_ghost_s.png")),
mMask([]()
      {
	      sf::ConvexShape* mask = new sf::ConvexShape(4u);
	      mask->setPoint(0u, sf::Vector2f(0.f, -25.f));
	      mask->setPoint(1u, sf::Vector2f(-55.f, 0.f));
	      mask->setPoint(2u, sf::Vector2f(0.f, 25.f));
	      mask->setPoint(3u, sf::Vector2f(55.f, 0.f));
	      return mask;
      }()),
mProjectileManager(projectileMgr),
mPath(path),
mFlock(flock),
mTowerCollisionGroup()
{
	mMask->setFillColor(sf::Color::Yellow);

	//set origin to be centre of shape
	//auto maskBounds = mMask->getLocalBounds();
	//mMask->setOrigin(maskBounds.width * 0.5f, maskBounds.height * 0.5f);

	auto overlayBounds = mOverlay.getLocalBounds();
	mOverlay.setOrigin(overlayBounds.width * 0.5f, overlayBounds.height * 0.85f);
}

TowerPlacer::~TowerPlacer() {
	delete mMask;
}

shared_ptr<tower::Tower> TowerPlacer::place() {
	shared_ptr<tower::Tower> placed = nullptr;

	//If tower placement is valid...
	if (mIsActive && mIsValid) {

		tinyxml2::XMLDocument doc;
		tinyxml2::XMLError result;
		if (mTowerType == ARROW) {

			result = doc.LoadFile(mArrowTowerDefPath.c_str());
			if (result != tinyxml2::XML_NO_ERROR) {
				throw result;
			}

			auto arrowTower = std::make_shared<tower::ProjectileTower>(mMask->getPosition(), doc.FirstChildElement("Tower"));
			arrowTower->setProjectileManager(mProjectileManager);
			placed = arrowTower;
		}

		else if (mTowerType == MAGIC) {

			result = doc.LoadFile(mMagicTowerDefPath.c_str());
			if (result != tinyxml2::XML_NO_ERROR) {
				throw result;
			}
			auto mageTower = std::make_shared<tower::MageTower>(mMask->getPosition(), doc.FirstChildElement("Tower"));
			mageTower->setProjectileManager(mProjectileManager);
			placed = mageTower;
		}

		else if (mTowerType == UNIT) {

			result = doc.LoadFile(mUnitTowerDefPath.c_str());
			if (result != tinyxml2::XML_NO_ERROR) {
				throw result;
			}

			auto unitTower = std::make_shared<tower::UnitTower>(mMask->getPosition(), doc.FirstChildElement("Tower"));
			unitTower->setPath(mPath);
			unitTower->setFlock(mFlock);
			placed = unitTower;
		}
		else {
			throw "INVALID TOWER TYPE (TowerPlacer::place())";
		}

		//Check if the tower placement intersects another tower...
		auto collidable = std::static_pointer_cast<collision::Collidable, tower::Tower>(placed);
		if (mTowerCollisionGroup.checkAgainst(collidable)) {
			//... if it does: don't place it.
			std::cout << "Tower placement intersects another tower!" << std::endl;
			placed = nullptr;
			mIsValid = false;
			calculateColor();
		}
		else {
			//... if it doesn't, add it to the collision group so that future towers can be tested against it.
			mTowerCollisionGroup.add(collidable);
			mIsActive = false;
		}
	}

	return placed;
}

void TowerPlacer::update(sf::Vector2i mousePosition) {
	if (mIsActive) {
		mMask->setPosition(mousePosition.x, mousePosition.y);
		mOverlay.setPosition(mMask->getPosition());

		checkValidity();
	}
}

void TowerPlacer::activate(TowerType type) {
	mIsActive = true;
	mTowerType = type;
}

void TowerPlacer::draw(sf::RenderTarget& renderTarget) const {
	if (mIsActive) {
		renderTarget.draw(mOverlay);

#ifdef _DEBUG
		renderTarget.draw(*mMask);
#endif
	}
}

bool TowerPlacer::isActive() const {
	return mIsActive;
}

void TowerPlacer::checkValidity() {
	if (mIsActive) {
		_ASSERT(mTerrainTree != nullptr);

		bool previouslyValid = mIsValid;
		mIsValid = true;

		//For each point of the shape...
		size_t numPoints = mMask->getPointCount();
		for (size_t index = 0u; index < numPoints; ++index) {

			//get the point of the shape, taking into account origin and position. (i.e. world bounds, not local bounds)
			sf::Vector2f point = mMask->getPoint(index) + mMask->getPosition() - mMask->getOrigin();

			unsigned char terrainAtPoint = TerrainInterpreter::NONE;

			//get data at point (returns false if point not in tree)
			if (!mTerrainTree->query(point.x, point.y, terrainAtPoint)) {
				mIsValid = false;
				break;
			} else if(terrainAtPoint & TerrainInterpreter::PATH) {	//if terrain at point contains any PATH(0x10)
				mIsValid = false;
				break;
			}
		}//end for

		//Change color if we need to...
		if (previouslyValid != mIsValid) {
			calculateColor();
		}
	}//end if(active)
}

void TowerPlacer::calculateColor() {
	_ASSERT(mIsActive);

	mOverlay.setColor(
		mIsValid ? mValidColor : mInvalidColor
		);
}