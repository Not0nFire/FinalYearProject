#include <include/Game.hpp>

#define GET_FONT(path) ResourceManager<sf::Font>::instance()->get(path)

Game::Game() :
	mRenderer(nullptr, sf::VideoMode(800U, 600U), "C00165681 - Final Year Project [WIP]"),
	mRun(false),
	mPaused(false),
	mSFGUI(new sfg::SFGUI())
{
	//create level scene
	//mLevel = new Level(&mRenderer.getWindow(), mSFGUI);

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError result = doc.LoadFile("./res/xml/levelOne.lvl");	//try to load the xml from file
	if (result != tinyxml2::XML_NO_ERROR)
		throw result;	//throw an error if one occured

	tinyxml2::XMLElement* root = doc.FirstChildElement("Level");
	mLevelOne = new Level(root, &(mRenderer.getWindow()), mSFGUI);
	SceneManager::instance()->createScene("LevelOne", mLevelOne);
	//mLevelOne->onLose.connect([this](){ mRun = false; });
	//mLevelOne->onWin.connect([](){ SceneManager::instance()->navigateToScene("LevelTwo"); });


	result = doc.LoadFile("./res/xml/levelTwo.lvl");
	if (result != tinyxml2::XML_NO_ERROR)
		throw result;	//throw an error if one occured
	root = doc.FirstChildElement("Level");
	mLevelTwo = new Level(root, &(mRenderer.getWindow()), mSFGUI);
	SceneManager::instance()->createScene("LevelTwo", mLevelTwo);
	//mLevelTwo->onLose.connect([this](){ mRun = false; });
	//mLevelTwo->onWin.connect([](){ SceneManager::instance()->navigateToScene("Menu"); });

	//create main menu scene
	mMenu = new Menu(mSFGUI);
	mMenu->addLabel("Main Menu");
	mMenu->addButton("Start", [](){ SceneManager::instance()->navigateToScene("LevelOne"); });
	//mMenu->addButton("Test", [](){std::cout << "test button clicked" << std::endl; });
	mMenu->addButton("Quit", bind(&MenuFunctions::exitProgram, "Quit"));

	SceneManager::instance()->createScene("Menu", mMenu);
	mRenderer.setScene(mMenu);

	//Tell the renderer to draw the correct scene whenever the scene changes
	SceneManager::instance()->onSceneChange.connect( bind(&Renderer::setScene, &mRenderer, _1) );
}

Game::~Game() {
	
}

int Game::run() {
	mRun = true;

	mRenderer.startRenderLoop(60U);

	sf::Event event;

	sf::Clock clock;
	sf::Time elapsedTime;

	while (mRun) {
		while (mRenderer.pollEvent(event)) {
			handleEvent(event);	//handle any events from the RenderWindow
		}//while pollEvent

		//get the elapsed time between this update and the last
		elapsedTime = clock.getElapsedTime();
		clock.restart();

		//do nothing if paused
		if (mPaused) {
			continue;
		}

		//Update stuff here...
		SceneManager::instance()->updateCurrentScene(elapsedTime);

		//Go to new scene if level is won/lost
		Level* lvl = dynamic_cast<Level*>(SceneManager::instance()->getEditableScene());
		if (lvl)
		{
			if (lvl->isLost())
			{
				SceneManager::instance()->navigateToScene("Menu");
			}
			else if (lvl->isWon())
			{
				SceneManager::instance()->navigateToScene(lvl->getNextScene());
			}
		}

	}//while mRun

	mRenderer.stopRenderLoop();

	std::cin.get();

	return EXIT_SUCCESS;
}

void Game::close() {
	mRun = false;
}

void Game::handleEvent(sf::Event& event) {

	if (SceneManager::instance()->passEventToCurrentScene(event)) {
		return; //event is already handled
	}

	switch (event.type) {

		case sf::Event::Closed:
			mRun = false;
			break;
		case sf::Event::LostFocus:
			mPaused = true;
			break;
		case sf::Event::GainedFocus:
			mPaused = false;
			break;
		default:
			break;
	}
}
