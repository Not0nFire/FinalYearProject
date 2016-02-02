#include <include/Level.hpp>
#include <include/Hero.hpp>
#include <include/ResourceManager.hpp>

#define GET_TEXTURE(path) ResourceManager<sf::Texture>::instance()->get(path)

bool Level::compareDepth(Actor* A, Actor* B) {
	return A->getPosition().y < B->getPosition().y;
}

#define GET_CHILD_VALUE(name) FirstChildElement(name)->GetText()	//make the code a little more readable
Level::Level(tinyxml2::XMLElement* root, sf::RenderWindow const* _relWindow, std::shared_ptr<sfg::SFGUI> sfgui) :
relWindow(_relWindow),
backgroundTEMP(GET_TEXTURE(root->GET_CHILD_VALUE("Background"))),
mHud(std::make_unique<HUD>(sfgui)),	//pass sfgui to HUD ctor and make HUD unique
mPath(root->FirstChildElement("Path")),
mLivesRemaining(atoi(root->GET_CHILD_VALUE("Lives"))),
mIsLost(false),
mIsWon(false),
mId(atoi(root->Attribute("id")))
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
			mHud->addHealthBarStatic(pawn, sf::Vector2f(10.f, 10.f), sf::Vector2f(200.f, 20.f));
		}
		else
		{
			pawn = factory.produce(type);
			mHud->addHealthBar(pawn, sf::Vector2f(-25.f, 35.f), sf::Vector2f(50.f, 5.f));
			dynamic_cast<Minion*>(pawn)->setPath(mPath.begin());
		}

		pawn->setDestination(pos);
		pawn->setPosition(pos);
		mCollisionGroup.add(pawn);
		mPawns.push_back(pawn);
	}

	for (Pawn* p : mPawns)
	{
		p->offerTarget(mHero);
	}

	mTowerPlacer = std::make_unique<TowerPlacer>(terrainTree, &mTowers, &mCollisionGroup);
}

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

	mIsWon = allPawnsDead;

	mCollisionGroup.check();


	for (auto tower : mTowers) {
		tower->update(elapsedTime);
		tower->acquireTarget(mPawns);
	}

	//mHud->update(elapsedTime);

	if (mIsLost)
	{
		onLose();
	}
	else if (mIsWon)
	{
		onWin();
	}
}//end update

void Level::draw(sf::RenderWindow &w) {
	boost::lock_guard<boost::mutex> lock(mMutex);

	w.draw(backgroundTEMP);

	//Perhaps this list should be a class member?
	std::list<Actor*> allActors;
	allActors.insert(allActors.end(), mPawns.begin(), mPawns.end());
	allActors.insert(allActors.end(), mTowers.begin(), mTowers.end());
	allActors.sort(&compareDepth);

	for (Actor* actor : allActors) {
		actor->debug_draw(w);
		actor->draw(w);
	}

	mTowerPlacer->update(sf::Mouse::getPosition(w));
	mTowerPlacer->draw(w);

	mHud->draw(w);
}

void Level::cleanup() {
	mHud->hide();
}

int Level::getID() const {
	return mId;
}