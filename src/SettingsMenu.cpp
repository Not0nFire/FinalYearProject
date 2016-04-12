#include <include/SettingsMenu.hpp>
#include <include/Constants.h>
#include <include/SceneManager.hpp>
#include <include/Settings.hpp>

SettingsMenu::SettingsMenu(tinyxml2::XMLElement* xml) :
mSaveChangesDialogueBox({ 400.f, 400.f }, { 200.f, 200.f }, "Save?", "Settings have changed.\nSave them?"),
mSettingsChanged(true)
{
	for (auto sliderXml = xml->FirstChildElement("Slider");
		nullptr != sliderXml;
		sliderXml = xml->NextSiblingElement("Slider"))
	{
		std::string key = sliderXml->Attribute("setting");
		//Map each slider to its setting
		mSliders.emplace(make_pair(key, gui::Slider(sliderXml)));
		mSliders.at(key).setPosition({ sliderXml->FloatAttribute("x"), sliderXml->FloatAttribute("y") });
	}

	for (auto btnXml = xml->FirstChildElement("Button");
		nullptr != btnXml;
		btnXml = xml->NextSiblingElement("Button"))
	{
		//Map each button to its setting
		mButtons.emplace(make_pair(std::string(btnXml->Attribute("setting")), gui::TextButton(btnXml->IntAttribute("x"), btnXml->IntAttribute("y"), btnXml)));
	}

	for (auto lblXml = xml->FirstChildElement("Label");
		nullptr != lblXml;
		lblXml = xml->NextSiblingElement("Label"))
	{
		auto label = sf::Text(lblXml->GetText(), ResourceManager<sf::Font>::instance()->get(Constants::Strings::getMainFontPath()));
		auto bounds = label.getLocalBounds();
		label.setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);

		label.setPosition(lblXml->FloatAttribute("x"), lblXml->FloatAttribute("y"));

		mLabels.push_back(std::move(label));
	}
}

SettingsMenu::~SettingsMenu() {}

bool SettingsMenu::handleEvent(sf::Event& Event) {
	bool handled = true;
	switch (Event.type) {
		case sf::Event::KeyPressed:
			if (Event.key.code == sf::Keyboard::Escape && mSettingsChanged) {
				SceneManager::instance()->showDialogueBox(&mSaveChangesDialogueBox);
			}
			break;

		case sf::Event::MouseButtonPressed:
			updateAll(Event.mouseButton.x, Event.mouseButton.y);
			break;

		case sf::Event::MouseMoved:
			updateAll(Event.mouseMove.x, Event.mouseMove.y);
			break;

		default:
			handled = false;
			break;
	}
	return handled;
}

void SettingsMenu::update(sf::Time const& elapsedTime) {}

void SettingsMenu::draw(sf::RenderWindow& w) {
	for (auto& entry : mSliders) {
		w.draw(entry.second);
		//TODO: apply change to setting
	}


	for (auto& entry : mButtons) {
		w.draw(entry.second);
		//TODO: apply change to setting
	}
}

void SettingsMenu::cleanup() {}

void SettingsMenu::updateAll(const int mouseX, const int mouseY) {
	for (auto& entry : mSliders) {
		entry.second.update(mouseX, mouseY);
		//TODO: apply change to setting
	}

	const sf::Vector2i mousePos(mouseX, mouseY);
	for (auto& entry : mButtons) {
		entry.second.update(mousePos);
		//TODO: apply change to setting
	}
}
