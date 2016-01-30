#include <include/Level.hpp>
#include <include/Hero.hpp>
#include <include/ResourceManager.hpp>

//using namespace tinyxml2;
#define GET_TEXTURE(path) ResourceManager<sf::Texture>::instance()->get(path)

//Level::Level(sf::RenderWindow const* _relWindow, std::shared_ptr<sfg::SFGUI> sfgui) :
//relWindow(_relWindow),
//backgroundTEMP( GET_TEXTURE("./res/img/bg.png") ),
//terrainTree(new TerrainTree(0, 0, 1000u, 1000u)),
//mTowerPlacer(terrainTree, &mTowers, &mCollisionGroup),
//mPath(/*put xml path here when tinyXML2 implemented*/),
//mHud(sfgui),
//mLivesRemaining(3),
//mBounds(sf::Vector2f(), sf::Vector2f(_relWindow->getSize().x, _relWindow->getSize().y)),
//mIsLost(false),
//mIsWon(false)
//{
//
//	mPawns.reserve(100);
//
//	mHero = new Hero(GET_TEXTURE("./res/img/placeholderActor.png"));
//	mHero->setPosition(mBounds.width * 0.5f, mBounds.height * 0.5f);
//	mPawns.push_back(mHero);
//
//	for (int i = 1; i < 10; i++) {
//		mPawns.push_back(new Minion(GET_TEXTURE("./res/img/placeholderActorBlue.png"), Pawn::Faction::ENEMY, mPath));
//		mPawns[i]->setPosition(mPath.begin()->getPoint());
//	}
//
//	for (Pawn* p : mPawns)
//	{
//		p->offerTarget(mHero);
//		mCollisionGroup.add(p);
//
//		if (p != mHero) {
//			mHud->addHealthBar(p, sf::Vector2f(-25.f, 35.f), sf::Vector2f(50.f, 5.f));
//		}
//	}
//
//
//	mHud->addHealthBarStatic(mHero, sf::Vector2f(10.f, 10.f), sf::Vector2f(200.f,20.f));
//
//	//Subdivide terrainTree
//	TerrainInterpreter interpreter = TerrainInterpreter("./res/img/terrain.bmp");
//	terrainTree->subdivide([interpreter](Quadtree<unsigned char>* node)
//	{
//		sf::IntRect nB = node->getBounds();
//
//		node->setData(interpreter.interpretArea(nB.left, nB.top, nB.width, nB.height));
//
//		if ((node->getData() & TerrainInterpreter::GRASS) &&	//If node contains grass and
//			(node->getData() & TerrainInterpreter::PATH) &&		//also contains path and
//			node->getLevel() < 10u)								//is less than 10 levels deep in the tree
//		{
//			return true;
//		}
//
//		return false;
//	});//end terrainTree subdivision
//}

Level::~Level() {
	for (auto itr = mPawns.begin(); itr != mPawns.end(); ++itr) {
		delete *itr;
	}
}

bool Level::handleEvent(sf::Event &event ) {
	boost::lock_guard<boost::mutex> lock(mMutex);

	bool handled = false;
	if (event.type == sf::Event::EventType::MouseButtonPressed) {
		if (mTowerPlacer->place()) {
			mCollisionGroup.add(*mTowers.rbegin());	//add the tower to collision group
			handled = true;
		} else {
			assert(relWindow != nullptr);
			mHero->setDestination(sf::Vector2f(sf::Mouse::getPosition(*relWindow)));
			handled = true;
		}

	} else if (event.type == sf::Event::EventType::KeyPressed && event.key.code == sf::Keyboard::T) {
		mTowerPlacer->activate();
		handled = true;

	}

	return handled;
}

void Level::update(sf::Time const &elapsedTime) {
	boost::lock_guard<boost::mutex> lock(mMutex);
	bool allPawnsDead = true;
	for (Pawn* p : mPawns) {

		p->update(elapsedTime);

		if (p != mHero && !p->isDead()) {
			allPawnsDead = false;


			if (p->getPosition().x > mBounds.left + mBounds.width) {

				//TODO: delete/erase pawn
				p->kill();

				if (--mLivesRemaining <= 0) {
					mIsLost = true;
				}
			}//if out of bounds

			else if (p->targetIsDead()) {
				for (Pawn* other : mPawns) {
					if (p->offerTarget(other)) {
						break;
					}//if
				}//for
			}//if target dead

		}//if !dead
	}//for

	mIsWon = allPawnsDead;	//update win condition

	mCollisionGroup.check();


	for (auto tower : mTowers) {
		tower->update(elapsedTime);
		tower->acquireTarget(mPawns);
	}

	//mHud->update(elapsedTime);
}//end update

void Level::draw(sf::RenderWindow &w) {
	boost::lock_guard<boost::mutex> lock(mMutex);

	w.draw(backgroundTEMP);

	//mHero->debug_draw(w);
	for (Pawn* p : mPawns) {
		p->debug_draw(w);
		w.draw(*p);
	}

	for (auto tower : mTowers) {
		tower->debug_draw(w);
		tower->draw(w);
	}

	mTowerPlacer->update(sf::Mouse::getPosition(w));
	mTowerPlacer->draw(w);

	mHud->draw(w);
}

void Level::cleanup() {
	mHud->hide();
}

bool Level::isLost() const {
	//return mIsLost;
	return false;
}

bool Level::isWon() const {
	//return mIsWon;
	return false;
}

#define GET_CHILD_VALUE(name) FirstChildElement(name)->GetText()	//make the code a little more readable
Level::Level(tinyxml2::XMLElement* root, sf::RenderWindow const* _relWindow, std::shared_ptr<sfg::SFGUI> sfgui) :
relWindow(_relWindow),
backgroundTEMP(GET_TEXTURE(root->GET_CHILD_VALUE("Background"))),
mHud(std::make_unique<HUD>(sfgui)),	//pass sfgui to HUD ctor and make HUD unique
mPath(root->FirstChildElement("Path")),
mLivesRemaining(atoi(root->GET_CHILD_VALUE("Lives"))),
mIsLost(false),
mIsWon(false)
{
	//Load <TerrainData> element and use it to instantiate the interpreter and subdivide the TerrainTree.
	//This has its own scope.
	{
		//instantiate the interpreter with the image path from the xml node
		TerrainInterpreter interpreter = TerrainInterpreter(root->GET_CHILD_VALUE("TerrainData"));

		//get the size of the image so we can construct our terrain tree.
		const sf::Vector2u imageSize = interpreter.getImageSize();

		mBounds = sf::FloatRect(0, 0, imageSize.x, imageSize.y);	//set the level bounds to match the image size

		//make a shared_ptr to the newly constructed terrain tree
		terrainTree = std::make_unique<TerrainTree>(TerrainTree(0, 0, imageSize.x, imageSize.y));

		//begin subdivision of terrain tree using the TerrainInterpreter
		terrainTree->subdivide([interpreter](Quadtree<unsigned char>* node)
		{
			sf::IntRect nB = node->getBounds();

			//Set the data of the node to correspond to the interpreters data
			node->setData(interpreter.interpretArea(nB.left, nB.top, nB.width, nB.height));

			//Subdivide this node if the following conditions are met:
			if ((node->getData() & TerrainInterpreter::GRASS) &&	//If node contains grass and
				(node->getData() & TerrainInterpreter::PATH) &&		//also contains path and
				node->getLevel() < 10u)								//is less than 10 levels deep in the tree
			{
				return true;
			}

			return false;
		});//end terrainTree subdivision lambda
	}//end terrain data scope

	//----------------------------------------------------------
	UnitFactory factory;
	//For each <Unit> element under the <Units> tag
	for (tinyxml2::XMLElement* enemyElement = root->FirstChildElement("Units")->FirstChildElement("Unit");
		enemyElement != nullptr;
		enemyElement = enemyElement->NextSiblingElement())
	{
		//get the position of the enemy from xml
		sf::Vector2f pos;
		pos.x = atof(enemyElement->GET_CHILD_VALUE("X"));
		pos.y = atof(enemyElement->GET_CHILD_VALUE("Y"));

		//get the type of enemy from xml
		std::string type = enemyElement->Attribute("type");

		
		Pawn * pawn;
		if (type == "hero")
		{
			pawn = factory.produce(type);
			mHero = dynamic_cast<Hero*>(pawn);
		}
		else
		{
			pawn = factory.produce(type);
			dynamic_cast<Minion*>(pawn)->setPath(mPath.begin());
		}
		mPawns.push_back(pawn);
	}

	for (Pawn* p : mPawns)
	{
		p->offerTarget(mHero);
		mCollisionGroup.add(p);

		if (p != mHero) {
			mHud->addHealthBar(p, sf::Vector2f(-25.f, 35.f), sf::Vector2f(50.f, 5.f));
		}
	}

	mHud->addHealthBarStatic(mHero, sf::Vector2f(10.f, 10.f), sf::Vector2f(200.f, 20.f));

	mTowerPlacer = std::make_unique<TowerPlacer>(terrainTree, &mTowers, &mCollisionGroup);
}