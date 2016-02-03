#include <include/Level.hpp>
#include <include/Hero.hpp>
#include <include/ResourceManager.hpp>

#define GET_TEXTURE(path) ResourceManager<sf::Texture>::instance()->get(path)
#define GET_FONT(path) ResourceManager<sf::Font>::instance()->get(path)
#define GET_SFX(path) ResourceManager<sf::SoundBuffer>::instance()->get(path)

bool Level::compareDepth(Actor* A, Actor* B) {
	return A->getPosition().y < B->getPosition().y;
}

#define GET_CHILD_VALUE(name) FirstChildElement(name)->GetText()	//make the code a little more readable
Level::Level(tinyxml2::XMLElement* root, sf::RenderWindow const* _relWindow, std::shared_ptr<sfg::SFGUI> sfgui) :
relWindow(_relWindow),
backgroundTEMP(GET_TEXTURE(root->GET_CHILD_VALUE("Background"))),
mHud(std::make_unique<HUD>(sfgui)),	//pass sfgui to HUD ctor and make HUD unique
mPath(root->FirstChildElement("Path")),
mLivesRemaining(std::make_shared<int>(atoi(root->GET_CHILD_VALUE("Lives")))),
mMoney(std::make_shared<int>(atoi(root->GET_CHILD_VALUE("StartingMoney")))),
mIsLost(false),
mIsWon(false),
mCamera(_relWindow->getSize(), sf::Vector2f(1200.f, 800.f)),
mId(atoi(root->Attribute("id"))),
mNextScene(root->GET_CHILD_VALUE("NextLevel")),
mMinionFlock(std::make_shared<std::list<Minion*>>()),
testUnitTower(GET_TEXTURE("./res/img/tower_sb.png"), sf::Vector2f(470, 180), mPath, "././res/xml/basic_ally.def", [this](Minion* m){mPawns.push_back(m); })
{
	
	mBgMusic.openFromFile(root->FirstChildElement("Music")->GetText());
	mBgMusic.setVolume(atoi(root->FirstChildElement("Music")->Attribute("volume")));	//read volume attribute from <Music> as an integer
	mBgMusic.setLoop(true);

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
			mHero = static_cast<Hero*>(pawn);
			mHud->addHealthBarStatic(pawn, sf::Vector2f(135.f, 46.f), sf::Vector2f(200.f, 35.f));
		}
		else
		{
			pawn = factory.produce(type);
			//mHud->addHealthBar(pawn, sf::Vector2f(-25.f, 35.f), sf::Vector2f(50.f, 5.f));	//Camera doesn't like moving healthbars
			Minion* minion = static_cast<Minion*>(pawn);
			minion->setPath(mPath.begin());
			minion->addToFlock(mMinionFlock);
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

	mHud->addImageWithLabel(GET_TEXTURE("./res/img/heart.png"), GET_FONT("./res/fonts/KENVECTOR_FUTURE.TTF"), sf::Vector2f(relWindow->getSize().x - 80.f, 10.f), sf::Vector2f(30.f, 0.f), mLivesRemaining);
	mHud->addImageWithLabel(GET_TEXTURE("./res/img/coin.png"), GET_FONT("./res/fonts/KENVECTOR_FUTURE.TTF"), sf::Vector2f(relWindow->getSize().x * 0.5f - 200.f, 2.5f), sf::Vector2f(30.f, 0.f), mMoney);
	mHud->addImage(GET_TEXTURE("./res/img/portrait.png"), sf::Vector2f());

	mTowerPlacer = std::make_unique<TowerPlacer>(terrainTree, &mTowers, &mCollisionGroup);

	mCamera.setTarget(mHero);

	mUnderlayTex.create(imageSize.x, imageSize.y);
	mUnderlaySpr.setTexture(mUnderlayTex.getTexture());
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
		if (*mMoney >= tower::BasicTower::getCost() && mTowerPlacer->place()) {
			*mMoney -= tower::BasicTower::getCost();
			mCollisionGroup.add(*mTowers.rbegin());	//add the tower to collision group
			handled = true;
		} else {
			assert(relWindow != nullptr);
			//destination = mouse position in window + camera position
			mHero->setDestination(sf::Vector2f(sf::Mouse::getPosition(*relWindow)) + mCamera.getDisplacement());
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
	bool allEnemiesDead = true;

	auto itr = mPawns.begin();
	while(itr != mPawns.end()) {
		Pawn* p = *itr;
		p->update(elapsedTime);

		if (p != mHero && !p->isDead()) {
			if (p->getFaction() == Pawn::Faction::ENEMY)
				allEnemiesDead = false;

			//if out of bounds
			if (!mBounds.contains(p->getPosition())) {

				//TODO: delete/erase pawn
				p->kill();

				if (--(*mLivesRemaining) <= 0) {
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

			++itr;

		}//if !dead
		else
		{
			//if dead and not playing an animation...
			if (!p->isPlayingAnimation() && p->isDead())
			{
				//draw to underlay and erase
				mUnderlayTex.draw(*p);
				mUnderlayTex.display();

				//award money for enemies that die
				if(p->getFaction() != Pawn::Faction::PLAYER) {
					*mMoney += static_cast<Minion*>(p)->getMonetaryValue();
				}

				itr = mPawns.erase(itr);
			}
			else
			{
				++itr;
			}

		}
	}//while

	mIsWon = allEnemiesDead;

	mCollisionGroup.check();


	for (auto tower : mTowers) {
		tower->update(elapsedTime);
		tower->acquireTarget(mPawns);
	}

	testUnitTower.update(elapsedTime);

	mCamera.update();

	if (mBgMusic.getStatus() != sf::Music::Status::Playing) {
		mBgMusic.play();
	}

	mHud->update(elapsedTime);

	mTowerPlacer->update(sf::Mouse::getPosition(*relWindow) + sf::Vector2i(mCamera.getDisplacement()));

	//if (mIsLost)
	//{
	//	onLose();
	//}
	//else if (mIsWon)
	//{
	//	onWin();
	//}
}//end update

void Level::draw(sf::RenderWindow &w) {
	boost::lock_guard<boost::mutex> lock(mMutex);
	
	w.setView(mCamera);

	w.draw(backgroundTEMP);

	w.draw(mUnderlaySpr);

	//Perhaps this list should be a class member?
	std::list<Actor*> allActors;
	allActors.insert(allActors.end(), mPawns.begin(), mPawns.end());
	allActors.insert(allActors.end(), mTowers.begin(), mTowers.end());
	allActors.push_back(&testUnitTower);
	allActors.sort(&compareDepth);

	for (Actor* actor : allActors) {
		actor->debug_draw(w);
		actor->draw(w);
	}

	mTowerPlacer->draw(w);

	mHud->draw(w);
}

bool Level::isWon() const {
	return mIsWon;
}

bool Level::isLost() const {
	return mIsLost;
}

void Level::cleanup() {
	mHud->hide();
	mBgMusic.stop();
}

int Level::getID() const {
	return mId;
}

std::string Level::getNextScene() const {
	return mNextScene;
}
