#include <include/Game.hpp>
#include <include/Settings.hpp>
#include <include/LevelSelect.hpp>
#include <include/MainMenu.hpp>

bool Game::mRun = false;
bool Game::mPaused = false;

std::unique_ptr<PlayerProfile> Game::mPlayerProfile = nullptr;

#define GET_FONT(path) ResourceManager<sf::Font>::instance()->get(path)

Game::Game() :
	mRenderer("C00165681 - Final Year Project [WIP]")
{
	Cursor::setTexture("./res/img/cursor.png");

	SceneManager::instance()->createScene<LevelSelect>("LevelSelect", "./res/xml/level_select.scene", false);

	SceneManager::instance()->createScene<MainMenu>("MainMenu", "./res/xml/main_menu.scene", true);

	mPlayerProfile = std::make_unique<PlayerProfile>("res/saves/profile.sav");

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

	SceneManager::destruct();

	//std::cin.get();

	return EXIT_SUCCESS;
}

void Game::close() {
	mRun = false;
}

PlayerProfile& Game::getPlayerProfile() {
	return *mPlayerProfile;
}

void Game::handleEvent(sf::Event& event) {

	if (event.type == sf::Event::EventType::KeyPressed && event.key.code == sf::Keyboard::Q) {
		Settings::set("Resolution", sf::Vector2i(800, 800));
	}

	if (event.type == sf::Event::EventType::MouseMoved) {
		//update cursor
		Cursor::setPosition(event.mouseMove.x, event.mouseMove.y);
	}

	if (!SceneManager::instance()->passEventToCurrentScene(event)) {
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
}
