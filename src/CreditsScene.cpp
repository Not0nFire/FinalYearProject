#include <include/CreditsScene.hpp>
#include <include/ResourceManager.hpp>
#include <include/SceneManager.hpp>


Credits::Credits(tinyxml2::XMLElement* root) :
mBackButton(
            root->FirstChildElement("BackButton")->IntAttribute("x"),
            root->FirstChildElement("BackButton")->IntAttribute("y"),
            root->FirstChildElement("BackButton")),
mBackdrop(ResourceManager<sf::Texture>::instance()->get(root->Attribute("backdrop"))),
mBaseScrollSpeed(root->FloatAttribute("baseScrollSpeed")),
mSpeedMultiplier(root->FloatAttribute("speedMultiplier")),
mSpeedUp(false)
{
	if (mBgMusic.openFromFile(root->Attribute("music"))) {
		//mBgMusic.setVolume(Settings::getInt("MusicVolume"));
		mBgMusic.setLoop(true);
		mBgMusic.play();
	}

	mRenderTex.create(800u, 4000u);
	mRenderTex.clear(sf::Color::Transparent);

	auto const& font = ResourceManager<sf::Font>::instance()->get("./res/fonts/NEVIS.TTF");
	sf::Color const textColor(73, 40, 17);

	auto index = 0u;
	for (auto entry = root->FirstChildElement("Entry");
		nullptr != entry;
		entry = entry->NextSiblingElement())
	{
		const int yPos = 500 * index;

		//`GET_ENTRY_VALUE(name)` === `entry->FirstChildElement(name)->GetText()`
		std::string const author = entry->FirstChildElement("Author")->GetText();
		std::string const link = entry->FirstChildElement("Link")->GetText();
		std::string const work = entry->FirstChildElement("Work")->GetText();
		
		auto authorTxt = sf::Text(author, font, 30u);
		auto bounds = authorTxt.getLocalBounds();
		authorTxt.setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);
		authorTxt.setPosition(400.f, yPos);
		authorTxt.setColor(textColor);

		mRenderTex.draw(authorTxt);

		auto linkTxt = sf::Text(link, font, 20u);
		bounds = linkTxt.getLocalBounds();
		linkTxt.setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);
		linkTxt.setPosition(400.f, yPos + 40);
		linkTxt.setColor(textColor);

		mRenderTex.draw(linkTxt);

		auto workTxt = sf::Text(work, font, 25u);
		bounds = workTxt.getLocalBounds();
		workTxt.setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);
		workTxt.setPosition(400.f, yPos + 70);
		workTxt.setColor(textColor);

		mRenderTex.draw(workTxt);

		auto licenceXml = entry->FirstChildElement("Licence");
		if (nullptr != licenceXml) {
			auto txt = sf::Text(licenceXml->GetText(), font, 20u);
			bounds = txt.getLocalBounds();
			txt.setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);
			txt.setPosition(400.f, yPos + 90);
			txt.setColor(textColor);

			mRenderTex.draw(txt);
		}

		auto imageXml = entry->FirstChildElement("Image");
		if (nullptr != imageXml) {
			sf::Texture tex;	//Don't need resource manager; Texture only needed briefly.
			tex.loadFromFile(imageXml->GetText());
			sf::Sprite img(tex);
			bounds = img.getLocalBounds();
			img.setOrigin(bounds.width * 0.5f, 0.f);
			img.setPosition(400.f, yPos + 110);

			mRenderTex.draw(img);
		}

		++index;
	}

	mRenderTex.display();
	mCredits.setTexture(mRenderTex.getTexture());

	mView.move(0.f, -1000.f);
}

Credits::~Credits() {}

bool Credits::handleEvent(sf::Event& evnt) {
	bool handled = true;
	switch (evnt.type) {
		case sf::Event::KeyPressed:
			if (evnt.key.code == sf::Keyboard::Escape) {
				SceneManager::instance()->navigateToScene("MainMenu");
			}
			mSpeedUp = true;
			break;

		case sf::Event::KeyReleased:
			mSpeedUp = false;
			break;

		case sf::Event::MouseWheelScrolled:
			//Scroll the credits
			mView.move(0.f, evnt.mouseWheelScroll.delta * -20.f);
			break;

		case sf::Event::MouseMoved:
			mBackButton.update(sf::Vector2i(evnt.mouseMove.x, evnt.mouseMove.y));
			break;

		case sf::Event::MouseButtonPressed:
			if (mBackButton.checkClick()) {
				SceneManager::instance()->navigateToScene("MainMenu");
			}
			mSpeedUp = true;
			break;

		case sf::Event::MouseButtonReleased:
			mSpeedUp = false;
			break;

		default:
			handled = false;
			break;
	}

	return handled;
}

void Credits::update(sf::Time const& elapsedTime) {

	//scroll until credits are done
	if (mView.getCenter().y < mCredits.getGlobalBounds().height) {
		//Scroll the view (and apply multipier if we should speed up)
		if (mSpeedUp) {
			mView.move(0.f, mBaseScrollSpeed * mSpeedMultiplier * elapsedTime.asSeconds());
		}
		else {
			mView.move(0.f, mBaseScrollSpeed * elapsedTime.asSeconds());
		}
	}
}

void Credits::draw(sf::RenderWindow& w) {
	w.draw(mBackdrop);
	w.draw(mBackButton);
	w.setView(mView);
	w.draw(mCredits);
}

void Credits::cleanup() {
	mBgMusic.stop();
}
