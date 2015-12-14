#include <include/Game.hpp>

#define GET_FONT(path) ResourceManager<sf::Font>::instance()->get(path)

Game::Game() :
	mRenderer(nullptr, sf::VideoMode(800U, 600U), "C00165681 - Final Year Project [WIP]"),
	mRun(false),
	mPaused(false)
{
	mLevel = new Level(&mRenderer.getWindow());
	SceneManager::instance()->createScene("Level", mLevel);

	mMenu = new Menu("Start", GET_FONT("./res/fonts/KENVECTOR_FUTURE.ttf"), &MenuFunctions::changeScene, sf::Vector2f(100.f, 100.f));
	mMenu->addItem("Quit", &MenuFunctions::exitProgram);

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

	}//while mRun

	mRenderer.stopRenderLoop();

	return EXIT_SUCCESS;
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