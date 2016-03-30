#include <include/Level.hpp>

#define GET_TEXTURE(path) ResourceManager<sf::Texture>::instance()->get(path)
#define GET_FONT(path) ResourceManager<sf::Font>::instance()->get(path)
#define GET_SFX(path) ResourceManager<sf::SoundBuffer>::instance()->get(path)

bool Level::compareDepth(std::shared_ptr<Actor> const &A, std::shared_ptr<Actor> const &B) {
	return A->getPosition().y < B->getPosition().y;
}

void Level::autofireProjectile(shared_ptr<Projectile> const& projectile) const {
	assert(!projectile->isActive());

	auto const& projectilePos = projectile->getPosition();
	auto& pawnList = *mPawns;
	auto nearestPawnDistance = std::numeric_limits<float>::max();
	shared_ptr<Pawn> target;
	
	//Find the nearest pawn
	for (auto const& pawn : pawnList)
	{
		if (pawn->getFaction() == Pawn::Faction::ENEMY) {
			auto distance = thor::length(pawn->getPosition() - projectilePos);

			if (distance < nearestPawnDistance)
			{
				nearestPawnDistance = distance;
				target = pawn;
			}
		}
	}

	if (target) {
		projectile->fire(projectilePos, target->getPosition(), 5.f);
		auto fancy = std::dynamic_pointer_cast<FancyProjectile, Projectile>(projectile);
		if (fancy)
		{
			fancy->setTarget(target);
		}
	}
}

#define GET_CHILD_VALUE(name) FirstChildElement(name)->GetText()	//make the code a little more readable

Level::Level(tinyxml2::XMLElement* root) :
mPawns(std::make_shared<std::list<shared_ptr<Pawn>>>()),
mCollisionGroup(new collision::CollisionGroup()),
mBackground(GET_TEXTURE(root->GET_CHILD_VALUE("Background"))),
mCamera(sf::Vector2u(800, 600), sf::Vector2f(1200.f, 800.f)),
mProjectileManager(new ProjectileManager(mCollisionGroup, GET_TEXTURE("./res/img/magic_particle.png"))),
mPath(std::make_shared<Path>(root->FirstChildElement("Path"))),
mMoney(std::make_shared<int>(atoi(root->GET_CHILD_VALUE("StartingMoney")))),
mLivesRemaining(std::make_shared<int>(atoi(root->GET_CHILD_VALUE("Lives")))),
mIsLost(false),
mIsWon(false),
mId(atoi(root->Attribute("id"))),
mNextScene(root->GET_CHILD_VALUE("NextLevel")),
mMinionFlock(std::make_shared<std::list<Minion*>>())
{
	
	mBgMusic.openFromFile(root->FirstChildElement("Music")->GetText());
	mBgMusic.setVolume(atof(root->FirstChildElement("Music")->Attribute("volume")));	//read volume attribute from <Music>
	mBgMusic.setLoop(true);

	//instantiate the interpreter with the image path from the xml node
	TerrainInterpreter interpreter = TerrainInterpreter(root->GET_CHILD_VALUE("TerrainData"));

	//get the size of the image so we can construct our terrain tree.
	const sf::Vector2u imageSize = interpreter.getImageSize();

	mBounds = sf::FloatRect(0, 0, imageSize.x, imageSize.y);	//set the level bounds to match the image size

	//make a shared_ptr to the newly constructed terrain tree
	terrainTree = std::make_unique<TerrainTree>(TerrainTree(0, 0, imageSize.x, imageSize.y));

	//begin subdivision of terrain tree using the TerrainInterpreter
	terrainTree->subdivide([&interpreter](Quadtree<unsigned char>* node)
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


		std::shared_ptr<Pawn> pawn;
		if (type == "hero")
		{
			pawn = factory.produce(type);

			mHero = pawn;
			//mHud->addHealthBarStatic(mHero.get(), sf::Vector2f(135.f, 46.f), sf::Vector2f(200.f, 35.f));
		}
		else
		{
			pawn = factory.produce(type);
			//mHud->addHealthBar(pawn, sf::Vector2f(-25.f, 35.f), sf::Vector2f(50.f, 5.f));	//Camera doesn't like moving healthbars
			auto minion = std::static_pointer_cast<Minion, Pawn>(pawn);
			auto constNode = std::const_pointer_cast<const Node>(mPath->begin());
			minion->setPath(constNode);
			minion->addToFlock(mMinionFlock);
		}

		pawn->setDestination(pos);
		pawn->setPosition(pos);
		mCollisionGroup->add(std::static_pointer_cast<collision::Collidable, Pawn>(pawn));
		mPawns->push_back(pawn);
	}

	for (auto& p : *mPawns)
	{
		p->offerTarget(mHero);
	}

	//mHud->addImageWithLabel(GET_TEXTURE("./res/img/heart.png"), GET_FONT("./res/fonts/KENVECTOR_FUTURE.TTF"), sf::Vector2f(720.f, 10.f), sf::Vector2f(30.f, 0.f), mLivesRemaining);
	//mHud->addImageWithLabel(GET_TEXTURE("./res/img/coin.png"), GET_FONT("./res/fonts/KENVECTOR_FUTURE.TTF"), sf::Vector2f(200.f, 2.5f), sf::Vector2f(30.f, 0.f), mMoney);
	//mHud->addImage(GET_TEXTURE("./res/img/portrait.png"), sf::Vector2f());

	mTowerPlacer = std::make_unique<TowerPlacer>(terrainTree, mProjectileManager, mPath, mMinionFlock);

	mCamera.setTarget(std::static_pointer_cast<Actor, Pawn>(mHero));

	mUnderlayTex.create(imageSize.x, imageSize.y);
	mUnderlaySpr.setTexture(mUnderlayTex.getTexture());

	mProjectileManager->setUnfiredProjectileHandler(bind(&Level::autofireProjectile, this, std::placeholders::_1));

	//Create abilities and their buttons.
	XMLDocument doc;
	doc.LoadFile("./res/xml/MagicMissileAbility.xml");
	auto abilityRoot = doc.FirstChildElement("Ability");

	mAbilityList.push_back(make_pair(
		gui::Button(100, 450, abilityRoot->FirstChildElement("Button")),
		std::make_unique<abilities::MagicMisile>(abilityRoot)
		));

	mAbilityList.rbegin()->second->setProjectileManager(mProjectileManager);
	mAbilityList.rbegin()->second->setPawnList(mPawns);
}

Level::~Level() {
}

bool Level::handleEvent(sf::Event &evnt ) {
	bool handled = false;
	if (evnt.type == sf::Event::EventType::MouseButtonPressed) {

		auto tower = mTowerPlacer->place();
		std::lock_guard<std::mutex> lock(mMutex);
		if (nullptr != tower) {

			if (*mMoney >= tower->getCost()) {
				*mMoney -= tower->getCost();
				mTowers.push_back(tower);
				mCollisionGroup->add(tower);	//add the tower to collision group
			}
			else {
				std::cout << "Not enough money to place tower!" << std::endl;
				//not enough money for tower
			}
			handled = true;

		} else {

			bool buttonClicked = false;
			for (auto& pair : mAbilityList) {
				//.first is the button
				//.second is the ability (unique_ptr)
				if (pair.first.checkClick()) {	//if the button was clicked...
					pair.second->execute(mHero.get());	//..execute the ability (as the hero)
					buttonClicked = true;
				}
			}

			if (!buttonClicked) {
				//destination = mouse position in window + camera position
				mHero->setDestination(sf::Vector2f(evnt.mouseButton.x, evnt.mouseButton.y) + mCamera.getDisplacement());
				handled = true;
			}
		}
	} else if (evnt.type == sf::Event::EventType::MouseMoved) {
		mTowerPlacer->update(sf::Vector2i(evnt.mouseMove.x, evnt.mouseMove.y) + sf::Vector2i(mCamera.getDisplacement()));

		for (auto& pair : mAbilityList) {
			//.first is the button
			//.second is the ability (unique_ptr)
			pair.first.update({evnt.mouseMove.x, evnt.mouseMove.y});	//update the button with the mouse position (as a Vector2i)
		}
	}
	else if (evnt.type == sf::Event::EventType::KeyPressed) {
		switch (evnt.key.code) {
		case sf::Keyboard::T:
			mTowerPlacer->activate(TowerPlacer::ARROW);
			handled = true;
			break;
		case sf::Keyboard::Y:
			mTowerPlacer->activate(TowerPlacer::MAGIC);
			handled = true;
			break;
		case sf::Keyboard::U:
			mTowerPlacer->activate(TowerPlacer::UNIT);
			handled = true;
			break;
		default:
			break;
		}
	}

	return handled;
}

void Level::update(sf::Time const &elapsedTime) {
	for (auto& pair : mAbilityList) {
		//.first is the button
		//.second is the ability (unique_ptr)
		pair.second->update(elapsedTime);
		if (pair.second->canCast()) {
			pair.first.enable();
		} else {
			pair.first.disable();
		}
	}

	bool allEnemiesDead = true;

	auto itr = mPawns->begin();
	auto end = mPawns->end();
	while (itr != end) {
		auto p = *itr;
		p->update(elapsedTime);

		if (p != mHero && !p->isDead()) {
			if (p->getFaction() == Pawn::Faction::ENEMY)
				allEnemiesDead = false;

			//if out of bounds
			if (!mBounds.contains(p->getPosition())) {

				p->kill();
				mCollisionGroup->remove(p);

				if (--(*mLivesRemaining) <= 0) {
					mIsLost = true;
				}

			}//if out of bounds

			else if (p->targetIsDead()) {
				for (auto &other : *mPawns) {
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
					*mMoney += std::static_pointer_cast<Minion, Pawn>(p)->getMonetaryValue();
				}

				itr = mPawns->erase(itr);
			}
			else
			{
				++itr;
			}

		}
	}//while

	mIsWon = allEnemiesDead;

	mCollisionGroup->check();


	for (auto tower : mTowers) {
		tower->update(elapsedTime);
		tower->shoot(mPawns);
	}

	mProjectileManager->update(elapsedTime);

	mCamera.update();

	if (mBgMusic.getStatus() != sf::Music::Status::Playing) {
		mBgMusic.play();
	}

	//mHud->update(elapsedTime);

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
	std::lock_guard<std::mutex> lock(mMutex);
	
	w.setView(mCamera);

	w.draw(mBackground);

	w.draw(mUnderlaySpr);

	//Perhaps this list should be a class member?
	std::list<shared_ptr<Actor>> allActors;
	for (auto itr = mPawns->begin(); itr != mPawns->end(); ++itr) {
		allActors.push_back(std::static_pointer_cast<Actor, Pawn>(*itr));
	}
	for (auto itr = mTowers.begin(); itr != mTowers.end(); ++itr) {
		allActors.push_back(std::static_pointer_cast<Actor, tower::Tower>(*itr));
	}
	allActors.sort(&compareDepth);

	for (auto &actor : allActors) {
		actor->debug_draw(w);
		actor->draw(w);
	}

	for (auto& pair : mAbilityList) {
		w.draw(*pair.second);	//draw the ability
	}

	mProjectileManager->draw(w);

	mTowerPlacer->draw(w);

	w.setView(w.getDefaultView());	//reset the view before we draw hud items

	for (auto& pair : mAbilityList) {
		w.draw(pair.first);	//draw the button
	}

	//mHud->draw(w);
}

bool Level::isWon() const {
	return mIsWon;
}

bool Level::isLost() const {
	return mIsLost;
}

void Level::cleanup() {
	std::lock_guard<std::mutex> lock(mMutex);
	//mHud->hide();
	mBgMusic.stop();
}

int Level::getID() const {
	return mId;
}

std::string Level::getNextScene() const {
	return mNextScene;
}
