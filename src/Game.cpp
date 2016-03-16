#include <include/Game.hpp>

bool Game::mRun = false;
bool Game::mPaused = false;

#define GET_FONT(path) ResourceManager<sf::Font>::instance()->get(path)

Game::Game() :
	mRenderer(nullptr, sf::VideoMode(800U, 600U), "C00165681 - Final Year Project [WIP]"),
	mSFGUI(new sfg::SFGUI())
{

	SceneManager::instance()->createScene<LevelSelect>("LevelSelect", "./res/xml/level_select.scene", false);

	//Tell the renderer to draw the correct scene whenever the scene changes
	SceneManager::instance()->onSceneChange.connect(bind(&Renderer::setScene, &mRenderer, _1));

	SceneManager::instance()->createScene<MainMenu>("MainMenu", "./res/xml/main_menu.scene", true);

	
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
		/*Level* lvl = dynamic_cast<Level*>(SceneManager::instance()->getEditableScene());
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
		}*/

	}//while mRun

	mRenderer.stopRenderLoop();

	//std::cin.get();

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
