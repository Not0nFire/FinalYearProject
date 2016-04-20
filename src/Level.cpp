#include <include/Level.hpp>
#include <include/Settings.hpp>
#include <include/SceneManager.hpp>
#include <include/Game.hpp>
#include <functional>

#define GET_TEXTURE(path) ResourceManager<sf::Texture>::instance()->get(path)
#define GET_FONT(path) ResourceManager<sf::Font>::instance()->get(path)
#define GET_SFX(path) ResourceManager<sf::SoundBuffer>::instance()->get(path)

void Level::drawToUnderlay(sf::Drawable const& drawable) {
	mUnderlayTex.draw(drawable);
	mUnderlayTex.display();
}

void Level::onPawnDeath(Pawn* pawn) {
	//award money for non-friendly pawns dying
	if (pawn->getFaction() != Pawn::Faction::PLAYER) {
		*mMoney += static_cast<Minion*>(pawn)->getMonetaryValue();
	}

	mBloodSystem.addSpurt(pawn->getPosition(), pawn->getBloodColor());
}

bool Level::compareDepth(shared_ptr<Actor> const &A, shared_ptr<Actor> const &B) {
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

bool Level::updatePawns(sf::Time const& elapsedTime) {
	bool allEnemiesDead = true;

	auto itr = mPawns->begin();
	auto end = mPawns->end();
	while (itr != end) {
		auto p = *itr;
		p->update(elapsedTime);

		if (p != mHero && !p->isDead()) {
			if (p->getFaction() == Pawn::Faction::ENEMY) {
				allEnemiesDead = false;
			}

			auto minion = std::dynamic_pointer_cast<Minion, Pawn>(p);
			//if reached end of path, kill the minion and decrement lives remaining
			if (nullptr != minion && minion->reachedEndOfPath()) {
				p->kill();
				--(*mLivesRemaining);

			}//if reached end of path
			else if (p->targetIsDead()) {
				//offer a new target if this one's is dead
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
				drawToUnderlay(*p);

				//award money for enemies that die
				if (p->getFaction() != Pawn::Faction::PLAYER) {
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

	return allEnemiesDead;
}

void Level::updateTowers(sf::Time const& elapsedTime) {
	for (auto tower : mTowers) {
		tower->update(elapsedTime);
		tower->shoot(mPawns);
	}
}

void Level::ensureMusicPlaying() {
	if (mBgMusic.getStatus() != sf::Music::Status::Playing) {
		mBgMusic.setVolume(Settings::getInt("MusicVolume"));
		mBgMusic.play();
	}
}

void Level::cleanPawnFlock() const {
	auto end = mFlock->end();
	auto itr = std::remove_if(
		mFlock->begin(),
		end,
		[](std::weak_ptr<Pawn> const& minion) { return minion.expired(); }	//remove if it has expired
	);

	if (itr != end) {
		mFlock->erase(itr, end);
	}
}

void Level::setupAbilities() {
	auto spawnCallback = bind(&Level::spawnMinion, this, std::placeholders::_1, false, true, true);

	/////////////////////////////////////////////////////
	//Create magic missile ability
	XMLDocument doc;
	auto result = doc.LoadFile("./res/xml/MagicMissileAbility.xml");
	if (result != XML_NO_ERROR) {
		throw result;
	}
	auto abilityRoot = doc.FirstChildElement("Ability");
	shared_ptr<Ability> ability = std::make_shared<abilities::MagicMisile>(abilityRoot);
	auto button = gui::AbilityButton(100, 650, abilityRoot->FirstChildElement("Button"), ability);

	mAbilityList.push_back(make_pair(button,ability));


	auto pair = mAbilityList.rbegin();
	pair->second->setProjectileManager(mProjectileManager);
	pair->second->setPawnList(mPawns);
	//ability.setSpawnCallback(spawnCallback);

	///////////////////////////////////////////////////////
	//Create raise dead ability
	result = doc.LoadFile("./res/xml/RaiseDeadAbility.xml");
	if (result != XML_NO_ERROR) {
		throw result;
	}
	abilityRoot = doc.FirstChildElement("Ability");
	ability = std::make_shared<abilities::RaiseDead>(abilityRoot);
	button = gui::AbilityButton(164, 650, abilityRoot->FirstChildElement("Button"), ability);

	mAbilityList.push_back(make_pair(button, ability));

	pair = mAbilityList.rbegin();
	pair->second->setProjectileManager(mProjectileManager);
	pair->second->setPawnList(mPawns);
	pair->second->setSpawnCallback(spawnCallback);

	///////////////////////////////////////////////////////
	//Create heal ability
	result = doc.LoadFile("./res/xml/HealAbility.xml");
	if (result != XML_NO_ERROR) {
		throw result;
	}
	abilityRoot = doc.FirstChildElement("Ability");
	ability = std::make_shared<abilities::Heal>(abilityRoot);
	button = gui::AbilityButton(228, 650, abilityRoot->FirstChildElement("Button"), ability);

	mAbilityList.push_back(make_pair(button, ability));
}

void Level::setupTowerButtons() {
	XMLDocument doc;
	auto const result = doc.LoadFile("./res/xml/towerButtons.xml");
	if (result != XML_NO_ERROR) {
		throw result;
	}
	const auto root = doc.FirstChildElement("TowerButtons");

	auto btnDef = root->FirstChildElement("Arrow");
	mTowerButtons[TowerPlacer::ARROW] = std::make_unique<gui::CostButton>(btnDef->IntAttribute("x"), btnDef->IntAttribute("y"), btnDef, mMoney);
	
	btnDef = root->FirstChildElement("Magic");
	mTowerButtons[TowerPlacer::MAGIC] = std::make_unique<gui::CostButton>(btnDef->IntAttribute("x"), btnDef->IntAttribute("y"), btnDef, mMoney);
	
	btnDef = root->FirstChildElement("Unit");
	mTowerButtons[TowerPlacer::UNIT] = std::make_unique<gui::CostButton>(btnDef->IntAttribute("x"), btnDef->IntAttribute("y"), btnDef, mMoney);
}

void Level::spawnMinion(shared_ptr<Minion> const& unit, bool setPath, bool addFlock, bool addCollision) {

	unit->makeSelfAware(std::static_pointer_cast<Pawn, Minion>(unit));
	
	if (setPath) {
		//Set path and position
		auto node = std::const_pointer_cast<const Node>(mPath->begin());
		auto spawnPos = node->getPoint();

		unit->setPath(node);

		unit->setPosition(spawnPos);
		unit->setDestination(spawnPos);
	}

	if (addFlock) {
		unit->addToFlock(mFlock);
	}

	if (addCollision) {
		mCollisionGroup->add(unit);
	}

	unit->setOnDeath(bind(&Level::onPawnDeath, this, std::placeholders::_1));

	mPawns->push_back(unit);
}

void Level::processPauseMenuResult() {
	switch (mPauseDialogue.getResult()) {

		//Do nothing if result is YES or CANCEL. (Resume or X clicked)
		case gui::DialogueBox::YES:
			std::cout << "Dialogue result: YES. Level resumed." << std::endl;
			break;

		//Quit the level if result is NO. (Quit clicked)
		case gui::DialogueBox::NO:
			std::cout << "Dialogue result: NO. Level quit." << std::endl;
			SceneManager::instance()->navigateToScene("LevelSelect");
			break;

		case gui::DialogueBox::CANCEL:
			std::cout << "Dialogue result: CANCEL. Level resumed." << std::endl;
			break;
	}
}

void Level::processFailDialogueResult() {
	switch (mFailDialogue.getResult()) {
		case gui::DialogueBox::YES:
			std::cout << "Dialogue result: YES. Return to LevelSelect" << std::endl;
			SceneManager::instance()->navigateToScene("LevelSelect");
			break;

		case gui::DialogueBox::NO:
			std::cout << "Dialogue result: NO. Return to MainMenu" << std::endl;
			SceneManager::instance()->navigateToScene("MainMenu");
			break;

		case gui::DialogueBox::CANCEL:
			std::cout << "Dialogue result: CANCEL. Level resumed." << std::endl;
			//Do nothing. Allow the user to sandbox in the level.
			break;
	}
}

void Level::processCompletionDialogueResult() {
	switch (mCompleteDialogue.getResult()) {

		case gui::DialogueBox::YES:
			std::cout << "Dialogue result: YES. Return to LevelSelect" << std::endl;
			SceneManager::instance()->navigateToScene("LevelSelect");
			break;

		case gui::DialogueBox::NO:
			std::cout << "Dialogue result: NO. Return to MainMenu" << std::endl;
			SceneManager::instance()->navigateToScene("MainMenu");
			break;

		case gui::DialogueBox::CANCEL:
			std::cout << "Dialogue result: CANCEL. Level resumed." << std::endl;
			//Do nothing. Allow the user to sandbox in the level.
			break;
	}
}

#define GET_CHILD_VALUE(name) FirstChildElement(name)->GetText()	//make the code a little more readable
Level::Level(XMLElement* root) :
mPauseDialogue({ 500.f, 400.f }, { 300.f, 300.f }, Constants::Strings::getPauseDialogueTitle(), Constants::Strings::getPauseDialogueBody(), Constants::Strings::getPauseDialogueYES(), Constants::Strings::getPauseDialogueNO()),
mCompleteDialogue({500.f,400.f}, {350.f, 400.f}, Constants::Strings::getCompletionDialogueTitle(), Constants::Strings::getCompletionDialogueBody(), Constants::Strings::getCompletionDialogueYES(), Constants::Strings::getCompletionDialogueNO()),
mFailDialogue({ 500.f, 400.f }, { 350.f, 400.f }, Constants::Strings::getFailDialogueTitle(), Constants::Strings::getFailDialogueBody(), Constants::Strings::getFailDialogueYES(), Constants::Strings::getFailDialogueNO()),
mPawns(std::make_shared<std::list<shared_ptr<Pawn>>>()),
mCollisionGroup(new collision::CollisionGroup()),
mBackground(GET_TEXTURE(root->GET_CHILD_VALUE("Background"))),
mCamera(sf::Vector2f(Settings::getVector2i("Resolution"))),
mProjectileManager(new ProjectileManager(mCollisionGroup, GET_TEXTURE("./res/img/magic_particle.png"))),
mPath(std::make_shared<Path>(root->FirstChildElement("Path"))),
mMoney(std::make_shared<int>(atoi(root->GET_CHILD_VALUE("StartingMoney")))),
mLivesRemaining(std::make_shared<int>(atoi(root->GET_CHILD_VALUE("Lives")))),
mNextLevel(root->GET_CHILD_VALUE("NextLevel")),
mFlock(std::make_shared<std::list<std::weak_ptr<Pawn>>>()),
mWaveController(root->FirstChildElement("WaveController"), bind(&Level::spawnMinion, this, std::placeholders::_1, true, true, true)),
mBloodSystem(GET_TEXTURE("./res/img/blood_particle.png"), bind(&Level::drawToUnderlay, this, std::placeholders::_1))
{
	//Let our camera translate the mouse.
	SceneManager::instance()->stopTranslatingMouse();

	mBgMusic.openFromFile(root->FirstChildElement("Music")->GetText());
	mBgMusic.setVolume(Settings::getInt("MusicVolume"));
	mBgMusic.setLoop(true);

	//instantiate the interpreter with the image path from the xml node
	TerrainInterpreter interpreter = TerrainInterpreter(root->GET_CHILD_VALUE("TerrainData"));

	//get the size of the image so we can construct our terrain tree.
	const sf::Vector2u imageSize = interpreter.getImageSize();

	mBounds = sf::FloatRect(0, 0, imageSize.x, imageSize.y);	//set the level bounds to match the image size

	mCamera.setBoundingArea(sf::Vector2f(mBounds.width, mBounds.height));

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

	mHud.addLifeTracker(mLivesRemaining, GET_TEXTURE(Constants::Strings::getLivesTexture()), Constants::Vectors::getLivesPosition(), Constants::Vectors::getLivesScale(), Constants::Vectors::getLivesSpacing());
	mHud.addImage(GET_TEXTURE(Constants::Strings::getMoneyTexture()), Constants::Vectors::getMoneyIconPosition());
	mHud.addNumberTracker(mMoney, Constants::Vectors::getMoneyPosition(), GET_FONT(Constants::Strings::getMainFontPath()));
	mHud.addImage(GET_TEXTURE(Constants::Strings::getPortraitTexture()), sf::Vector2f());


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
			mFlock->push_back(pawn);	//Add the hero to the flock. The hero doesn't do any flock logic but this allows minions to account for him.
			mHud.addHealthBar(mHero,sf::Vector2f(135.f, 46.f), sf::Vector2f(200.f, 35.f), &GET_TEXTURE("./res/img/hp_red.png"),true);
		}
		else
		{
			pawn = factory.produce(type);
			//mHud.addHealthBar(pawn, sf::Vector2f(-25.f, 35.f), sf::Vector2f(50.f, 5.f));
			auto minion = std::static_pointer_cast<Minion, Pawn>(pawn);
			auto constNode = mPath->begin();
			minion->setPath(constNode);
			minion->addToFlock(mFlock);
		}

		pawn->setDestination(pos);
		pawn->setPosition(pos);
		mCollisionGroup->add(std::static_pointer_cast<collision::Collidable, Pawn>(pawn));
		mPawns->push_back(pawn);
	}

	for (auto& p : *mPawns)
	{
		p->offerTarget(mHero);
		p->setOnDeath(bind(&Level::onPawnDeath, this, std::placeholders::_1));
	}

	mTowerPlacer = std::make_unique<TowerPlacer>(terrainTree, mProjectileManager, mPath, bind(&Level::spawnMinion, this, std::placeholders::_1, false, true, true));

	mCamera.setTarget(std::static_pointer_cast<Actor, Pawn>(mHero));

	mUnderlayTex.create(imageSize.x, imageSize.y);
	mUnderlaySpr.setTexture(mUnderlayTex.getTexture());

	mProjectileManager->setUnfiredProjectileHandler(bind(&Level::autofireProjectile, this, std::placeholders::_1));

	setupAbilities();

	setupTowerButtons();
}

Level::~Level() {
	mBgMusic.stop();
}

bool Level::handleEvent(sf::Event &evnt ) {
	bool handled = false;
	if (evnt.type == sf::Event::EventType::MouseButtonPressed) {
		handled = true;

		auto tower = mTowerPlacer->place();
		//std::lock_guard<std::mutex> lock(mMutex);
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

			//Check ability buttons
			bool buttonClicked = false;
			for (auto& pair : mAbilityList) {
				//.first is the button
				//.second is the ability (unique_ptr)
				if (!buttonClicked) {
					buttonClicked = pair.first.containsMouse();
				}

				if (pair.first.checkClick()) {	//if the button was clicked and not disabled...
					pair.second->execute(mHero.get());	//..execute the ability (as the hero)
				}

				if (buttonClicked) {
					break;
				}
			}

			//Check tower buttons
			if (!buttonClicked) {
				for (auto const& entry : mTowerButtons) {
					//.first is enum key
					//.second is button
					buttonClicked = entry.second->containsMouse();
					if (entry.second->checkClick()) {
						mTowerPlacer->activate(entry.first);
					}

					if (buttonClicked) {
						break;
					}
				}
			}

			if (!buttonClicked) {
				//destination = mouse position in window + camera position

				mHero->setDestination(mCamera.screenPositionToGamePosition(evnt.mouseButton.x, evnt.mouseButton.y));
				handled = true;
			}
		}

	} else if (evnt.type == sf::Event::EventType::MouseMoved) {
		handled = true;

		mTowerPlacer->update(mCamera.screenPositionToGamePosition(evnt.mouseMove.x, evnt.mouseMove.y));

		//use default camera for gui items
		const sf::Vector2i mousePos(Camera::getDefaultCamera().screenPositionToGamePosition(evnt.mouseMove.x, evnt.mouseMove.y));

		for (auto& pair : mAbilityList) {
			//.first is the button
			//.second is the ability (unique_ptr)
			pair.first.update(mousePos);	//update the button with the mouse position (as a Vector2i)
		}

		mTowerButtons[TowerPlacer::ARROW]->update(mousePos);
		mTowerButtons[TowerPlacer::MAGIC]->update(mousePos);
		mTowerButtons[TowerPlacer::UNIT]->update(mousePos);

		mCamera.doMouseMove(sf::Vector2f(evnt.mouseMove.x, evnt.mouseMove.y));
	}//end mouse press handling

	//Handle key presses
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

		case sf::Keyboard::Space:
			mCamera.toggleLock();

		case sf::Keyboard::RShift:
		case sf::Keyboard::LShift:
			mTowerPlacer->setSticky(true);
			break;

		case sf::Keyboard::Escape:
			SceneManager::instance()->showDialogueBox(&mPauseDialogue);

		default:
			break;
		}
	}//end key press handling

	//Handle key releases
	else if (evnt.type == sf::Event::EventType::KeyReleased) {
		switch (evnt.key.code) {
		case sf::Keyboard::RShift:
		case sf::Keyboard::LShift:
			mTowerPlacer->setSticky(false);
			break;
		default:
			break;
		}
	}//end key release handling

	//Process text input (for ability hotkeys)
	else if (evnt.type == sf::Event::EventType::TextEntered) {
		//If it's less than 128 we can cast it to a char
		if (evnt.text.unicode < 128) {
			char key = evnt.text.unicode;	//Cast to char

			//Check hotkey for each ability
			for (auto &pair : mAbilityList) {
				if (pair.second->checkHotkey(key)) {
					pair.second->execute(mHero.get());
				}
			}
		}
	}//end text input handling

	return handled;
}

void Level::update(sf::Time const &elapsedTime) {

	//If  getResult() has not been called on the dialogue box...
	if (!mPauseDialogue.resultProcessed()) {
		processPauseMenuResult();
	}
	else if (!mFailDialogue.resultProcessed()) {
		processFailDialogueResult();
	}
	else if (!mCompleteDialogue.resultProcessed()) {
		processCompletionDialogueResult();
	}
	else {

		for (auto& pair : mAbilityList) {
			//.first is the button
			//.second is the ability (shared_ptr)
			pair.first.updateCooldownVisuals(elapsedTime);
			pair.second->update(elapsedTime);
		}

		mWaveController.update(elapsedTime);


		//updatePawns() returns true if all enemy pawns are dead
		if (updatePawns(elapsedTime) && mWaveController.isEmpty()) {
			Game::getPlayerProfile().unlockLevel(mNextLevel);
			SceneManager::instance()->showDialogueBox(&mCompleteDialogue);
		}
		else if (*mLivesRemaining <= 0) {
			SceneManager::instance()->showDialogueBox(&mFailDialogue);
		}

		mCollisionGroup->check(elapsedTime.asSeconds());

		updateTowers(elapsedTime);

		mProjectileManager->update(elapsedTime);

		cleanPawnFlock();

		mCamera.update(elapsedTime.asSeconds());

		ensureMusicPlaying();

		mBloodSystem.update(elapsedTime);

		mHud.update(elapsedTime.asSeconds());
	}

}//end update

void Level::draw(sf::RenderWindow &w) {
	//std::lock_guard<std::mutex> lock(mMutex);
	
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

	w.draw(mBloodSystem);

	mTowerPlacer->draw(w);

	w.setView(Camera::getDefaultCamera());	//reset the view before we draw hud items

	for (auto& pair : mAbilityList) {
		w.draw(pair.first);	//draw the button
	}

	w.draw(*mTowerButtons[TowerPlacer::ARROW]);
	w.draw(*mTowerButtons[TowerPlacer::MAGIC]);
	w.draw(*mTowerButtons[TowerPlacer::UNIT]);

	w.draw(mHud);
}

void Level::cleanup() {
	//std::lock_guard<std::mutex> lock(mMutex);
	//mHud->hide();
	mBgMusic.stop();
}
