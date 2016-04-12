#include <include/MainMenu.hpp>
#include <include/Game.hpp>
#include <include/Settings.hpp>

MainMenu::MainMenu(tinyxml2::XMLElement* root) :
mQuitConfirmDialogue({ 400.f, 400.f }, { 300.f, 300.f }, "Quit", "Are you sure?")
{
	_ASSERT(root->Name() == std::string("MainMenu"));

	mBackground.setTexture(
		ResourceManager<sf::Texture>::instance()->get(
			root->FirstChildElement("Backdrop")->Attribute("path")
		)//end get()
	);//end setTexture()

	mMusic.openFromFile(
		root->FirstChildElement("Music")->Attribute("path")
	);//end openFromFile()

	mMusic.setLoop(true);
	
	auto startBtnXml = root->FirstChildElement("StartButton");
	mStartButton = std::make_unique<gui::Button>(
		atoi(startBtnXml->Attribute("x")),
		atoi(startBtnXml->Attribute("y")),
		startBtnXml
	);

	auto quitBtnXml = root->FirstChildElement("QuitButton");
	mQuitButton = std::make_unique<gui::Button>(
		atoi(quitBtnXml->Attribute("x")),
		atoi(quitBtnXml->Attribute("y")),
		quitBtnXml
	);
}

MainMenu::~MainMenu() {}

bool MainMenu::handleEvent(sf::Event& evnt) {
	bool handled = false;

	switch (evnt.type) {
		default:
			break;
		case sf::Event::MouseButtonPressed:
			handled = true;
			if (mStartButton->checkClick()) {
				//start the game
				SceneManager::instance()->navigateToScene("LevelSelect");
			} else if (mQuitButton->checkClick()) {
				//quit the game
				SceneManager::instance()->showDialogueBox(&mQuitConfirmDialogue);
			}
			break;
		case sf::Event::MouseButtonReleased:
			break;
		case sf::Event::MouseMoved:
			auto mousePos = sf::Vector2i(evnt.mouseMove.x, evnt.mouseMove.y);
			mStartButton->update(mousePos);
			mQuitButton->update(mousePos);
			handled = true;
			break;
	}

	return handled;
}

void MainMenu::update(sf::Time const& elapsedTime) {
	if (mMusic.getStatus() != sf::Music::Status::Playing) {
		mMusic.play();
	}

	if (!mQuitConfirmDialogue.resultProcessed()) {
		if (mQuitConfirmDialogue.getResult() == gui::DialogueBox::YES) {
			cleanup();
			Game::close();
		}
	}
}

void MainMenu::draw(sf::RenderWindow &w) {
	w.draw(mBackground);
	w.draw(*mStartButton);
	w.draw(*mQuitButton);
}

void MainMenu::cleanup() {
	mMusic.stop();
}
