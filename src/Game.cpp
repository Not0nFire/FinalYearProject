#include <include/Game.hpp>

#define GET_FONT(path) ResourceManager<sf::Font>::instance()->get(path)

Game::Game() :
	mRenderer(nullptr, sf::VideoMode(800U, 600U), "C00165681 - Final Year Project [WIP]"),
	mRun(false),
	mPaused(false)
{
	mLevel = new Level();
	SceneManager::instance()->createScene("Level", mLevel);
	onMouseClick.connect(boost::bind(&Pawn::setDestination, mLevel->getHero(), _1));

	mMenu = new Menu();
	mMenu->addLabel("Main Menu");
	mMenu->addButton("Start", bind(&MenuFunctions::changeScene, "Level"));
	mMenu->addButton("Quit", bind(&MenuFunctions::exitProgram, "Quit"));

	SceneManager::instance()->createScene("Menu", mMenu);
	mRenderer.setScene(mMenu);

	//It's dirty but the deadline is hours away. To be cleaned in sprint 2.
	SceneManager::instance()->onSceneChange.connect([this](std::string str)
	{
		if (str == "Level") {
			mRenderer.setScene(mLevel);
		}
		else if (str == "Menu") {
			mRenderer.setScene(mMenu);
		}
	});
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

	}//while mRun

	mRenderer.stopRenderLoop();

	return EXIT_SUCCESS;
}

void Game::handleEvent(sf::Event& event) {

	switch (event.type) {

		case sf::Event::Closed:
			mRun = false;
			break;
		case sf::Event::Resized: break;
		case sf::Event::LostFocus:
			mPaused = true;
			break;
		case sf::Event::GainedFocus:
			mPaused = false;
			break;
		case sf::Event::TextEntered: break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape) {
				SceneManager::instance()->navigateToScene("Menu");
			}
			else {
				SceneManager::instance()->passEventToCurrentScene(event);
			}
			break;
		case sf::Event::KeyReleased: break;
		case sf::Event::MouseWheelMoved: break;
		case sf::Event::MouseWheelScrolled: break;
		case sf::Event::MouseButtonPressed:
			std::cout << "Click!" << std::endl;
			//Activate onClick signal, passing mouse position relative to the render window
			onMouseClick(sf::Vector2f(sf::Mouse::getPosition(mRenderer.getWindow())));
			break;
		case sf::Event::MouseButtonReleased: break;
		case sf::Event::MouseMoved: break;
		case sf::Event::MouseEntered: break;
		case sf::Event::MouseLeft: break;
		case sf::Event::JoystickButtonPressed: break;
		case sf::Event::JoystickButtonReleased: break;
		case sf::Event::JoystickMoved: break;
		case sf::Event::JoystickConnected: break;
		case sf::Event::JoystickDisconnected: break;
		case sf::Event::TouchBegan: break;
		case sf::Event::TouchMoved: break;
		case sf::Event::TouchEnded: break;
		case sf::Event::SensorChanged: break;
		case sf::Event::Count: break;
		default: break;
	}
}