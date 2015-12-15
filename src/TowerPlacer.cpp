#include <include/Towers/TowerPlacer.hpp>

const sf::Color TowerPlacer::mValidColor = sf::Color::Green;
const sf::Color TowerPlacer::mInvalidColor = sf::Color::Red;

TowerPlacer::TowerPlacer(std::shared_ptr<TerrainTree> terrainTree, std::vector<tower::BasicTower*> *towerContainer, collision::CollisionGroup* collisionGroup) :
mIsActive(false),
mIsValid(false),
mTerrainTree(terrainTree),
mOverlay(ResourceManager<sf::Texture>::instance()->get("./res/img/tower_ghost.png")),
mMask(new sf::CircleShape(40, 4u)),
mTowerContainer(towerContainer),
towerProjectileCollisionGroup(collisionGroup)
{
	mMask->setFillColor(sf::Color::Yellow);

	//set origin to be centre of shape
	auto maskBounds = mMask->getLocalBounds();
	mMask->setOrigin(maskBounds.width * 0.5f, maskBounds.height * 0.5f);

	auto overlayBounds = mOverlay.getLocalBounds();
	mOverlay.setOrigin(overlayBounds.width * 0.5f, overlayBounds.height * 0.85f);
}

TowerPlacer::~TowerPlacer() {
	delete mMask;
}

bool TowerPlacer::place() {
	bool placed = false;

	//If tower placement is valid...
	if (mIsActive && mIsValid) {
		///...put a tower in the container
		mTowerContainer->push_back(new tower::BasicTower(
			ResourceManager<sf::Texture>::instance()->get("./res/img/tower.png"),
			mMask->getPosition(), 300.0f, 1.0f, 10,
			Damage::Type::PHYSICAL,
			towerProjectileCollisionGroup
			));

		placed = true;
		mIsActive = false;
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

void TowerPlacer::activate() {
	mIsActive = true;
}

void TowerPlacer::draw(sf::RenderTarget& renderTarget) {
	if (mIsActive) {
		renderTarget.draw(mOverlay);

#ifdef _DEBUG
		renderTarget.draw(*mMask);
#endif
	}
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