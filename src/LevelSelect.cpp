#include "include/LevelSelect.hpp"

LevelSelect::LevelSelect(tinyxml2::XMLElement* xml) {
	
	_ASSERT(xml->Name() == std::string("LevelSelect"));

	mBackground.setTexture(
		ResourceManager<sf::Texture>::instance()->get(
			xml->FirstChildElement("Backdrop")->Attribute("path")
		)//end get()
	);//end setTexture()

	mMusic.openFromFile(
		xml->FirstChildElement("Music")->Attribute("path")
	);//end openFromFile()

	mMusic.setLoop(true);

	auto backBtnXml = xml->FirstChildElement("BackButton");
	mBackButton = std::make_unique<gui::Button>(
		atoi(backBtnXml->Attribute("x")),
		atoi(backBtnXml->Attribute("y")),
		backBtnXml
	);
	
	//Create all the buttons for the levels
	for (	auto lvlBtnXml = xml->FirstChildElement("LevelButtons")->FirstChildElement("Button");
			lvlBtnXml != nullptr;
			lvlBtnXml = lvlBtnXml->NextSiblingElement("Button")
		) {

		//Each list entry is a button and a path to where its xml level definition is.
		//We use move because we can't copy a unique_ptr.
		//We use make_pair to contruct our <button,string> entry
		//We use make_unique to construct or unique button pointer.
		mLevelButtons.push_back(
			std::move(
				std::make_pair(
					std::make_unique<gui::Button>(
						atoi(lvlBtnXml->Attribute("x")),
						atoi(lvlBtnXml->Attribute("y")),
						lvlBtnXml
					),//end make_unique
					lvlBtnXml->Attribute("path")
				)//end make_pair
			)//end move
		);//end emplace_back
	}//end for
}

LevelSelect::~LevelSelect() {
}

bool LevelSelect::handleEvent(sf::Event& evnt) {
	bool handled = false;
	switch (evnt.type) {
		case sf::Event::MouseButtonPressed:
			handled = true;
			if (mBackButton->checkClick()) {
				SceneManager::instance()->navigateToScene("MainMenu");
				break;
			}
			for (auto itr = mLevelButtons.begin(); itr != mLevelButtons.end(); ++itr) {
				if (itr->first->checkClick()) {
					SceneManager::instance()->createScene<Level>("test_level", "./res/xml/levelOne.lvl");
					break;
				}
			}
			break;
		case sf::Event::MouseMoved:
			handled = true;
			auto mousePos = sf::Vector2i(evnt.mouseMove.x, evnt.mouseMove.y);
			mBackButton->update(mousePos);
			for (auto itr = mLevelButtons.begin(); itr != mLevelButtons.end(); ++itr) {
				itr->first->update(mousePos);
			}
			break;
	}

	return handled;
}

void LevelSelect::update(sf::Time const& elapsedTime) {
	if (mMusic.getStatus() != sf::Music::Status::Playing) {
		mMusic.play();
	}
}

void LevelSelect::draw(sf::RenderWindow& w) {
	w.draw(mBackground);
	mBackButton->draw(w);
	for (auto itr = mLevelButtons.begin(); itr != mLevelButtons.end(); ++itr) {
		itr->first->draw(w);
	}
}

void LevelSelect::cleanup() {
	mMusic.stop();
}
