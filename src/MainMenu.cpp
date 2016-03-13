#include <include/MainMenu.hpp>

MainMenu::MainMenu(tinyxml2::XMLElement* root) {
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
	mStartButton = gui::Button(
		atoi(startBtnXml->Attribute("x")),
		atoi(startBtnXml->Attribute("y")),
		startBtnXml
	);

	auto quitBtnXml = root->FirstChildElement("QuitButton");
	mQuitButton = gui::Button(
		atoi(quitBtnXml->Attribute("x")),
		atoi(quitBtnXml->Attribute("y")),
		quitBtnXml
	);
}

MainMenu::~MainMenu() {}

bool MainMenu::handleEvent(sf::Event& Event) {
	bool handled = false;

	switch (Event.type) {
		case sf::Event::MouseButtonPressed:
			handled = true;
			if (mStartButton.checkClick()) {
				//start the game
				SceneManager::instance()->navigateToScene("LevelOne");
			} else if (mQuitButton.checkClick()) {
				//quit the game
				Game::close();
			}
			break;
		case sf::Event::MouseButtonReleased:
			break;
		case sf::Event::MouseMoved:
			//if weak_ptr holds a window...
			if (auto window = mWindow.lock()) {
				//...update the buttons with relative mouse position
				auto mousePos = sf::Mouse::getPosition(*window);
				mStartButton.update(mousePos);
				mQuitButton.update(mousePos);
				handled = true;
			}
			break;
		default:
			break;
	}

	return handled;
}

void MainMenu::update(sf::Time const& elapsedTime) {}

void MainMenu::draw(sf::RenderWindow& w) {
	if (mWindow.expired()) {
		//mWindow = w;	!!!all scenes will need to be changed to use weak ptrs!!!
	}

	mStartButton.draw(w);
}

void MainMenu::cleanup() {
	mMusic.stop();
}
