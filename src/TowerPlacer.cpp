#include <include/Towers/TowerPlacer.hpp>

const sf::Color TowerPlacer::mValidColor = sf::Color::Green;
const sf::Color TowerPlacer::mInvalidColor = sf::Color::Red;

TowerPlacer::TowerPlacer(std::shared_ptr<TerrainTree> terrainTree, std::vector<tower::BasicTower*> *towerContainer) :
mIsActive(false),
mIsValid(false),
mTerrainTree(terrainTree),
mOverlay(ResourceManager<sf::Texture>::instance()->get("./res/img/tower_ghost.png")),
mMask(new sf::CircleShape(40, 4u)),
mTowerContainer(towerContainer)
{
	mMask->setFillColor(sf::Color::Yellow);

	//set origin to be centre of shape
	auto maskBounds = mMask->getLocalBounds();
	mMask->setOrigin(maskBounds.width * 0.5f, maskBounds.height * 0.5f);

	auto overlayBounds = mOverlay.getLocalBounds();
	mOverlay.setOrigin(overlayBounds.width * 0.5f, overlayBounds.height * 0.8f);
}

TowerPlacer::~TowerPlacer() {
	delete mMask;
}

bool TowerPlacer::place() {
	bool placed = false;

	//If tower placement is valid...
	if (mIsActive && mIsValid) {
		///...emplace the tower in the container
		/*mTowerContainer->emplace_back(
			ResourceManager<sf::Texture>::instance()->get("./res/img/tower.png"),
			sf::Vector2f(500, 300), 300.0f, 1.0f, 10,
			Damage::Type::PHYSICAL
			);*/

		placed = true;
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
	renderTarget.draw(mOverlay);

#ifdef _DEBUG
	renderTarget.draw(*mMask);
#endif
}

void TowerPlacer::checkValidity() {
	if (mIsActive) {
		_ASSERT(mTerrainTree != nullptr);

		bool previouslyValid = mIsValid;
		mIsValid = true;

		size_t numPoints = mMask->getPointCount();
		for (size_t index = 0u; index < numPoints; ++index) {

			sf::Vector2f point = mMask->getPoint(index) + mMask->getPosition() - mMask->getOrigin();
			unsigned char terrainAtPoint = 0x00;

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