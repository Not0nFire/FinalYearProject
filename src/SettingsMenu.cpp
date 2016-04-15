#include <include/SettingsMenu.hpp>
#include <include/Constants.h>
#include <include/SceneManager.hpp>
#include <include/Settings.hpp>

SettingsMenu::SettingsMenu(tinyxml2::XMLElement* xml) :
mSaveChangesDialogueBox({ 400.f, 400.f }, { 400.f, 300.f }, "Save?", "Settings have changed.\nSave them?"),
mSettingsChanged(false),
mBackdrop(ResourceManager<sf::Texture>::instance()->get(xml->FirstChildElement("Backdrop")->Attribute("path"))),
mBackButton(xml->FirstChildElement("QuitButton")->IntAttribute("x"), xml->FirstChildElement("QuitButton")->IntAttribute("y"), xml->FirstChildElement("QuitButton"))
{
	std::cout << "Creating settings menu...\n";

	std::cout << "\tParsing Sliders...\n";
	for (auto sliderXml = xml->FirstChildElement("Slider");
		nullptr != sliderXml;
		sliderXml = sliderXml->NextSiblingElement("Slider"))
	{
		std::string key = sliderXml->Attribute("setting");
		std::cout << "\t\tAdding slider for setting \"" << key <<"\"...\n";
		//Map each slider to its setting
		mSliders.emplace(make_pair(key, gui::Slider(sliderXml)));
		mSliders.at(key).setPosition({ sliderXml->FloatAttribute("x"), sliderXml->FloatAttribute("y") });
	}

	std::cout << "\tParsing Buttons...\n";
	for (auto btnXml = xml->FirstChildElement("Button");
		nullptr != btnXml;
		btnXml = btnXml->NextSiblingElement("Button"))
	{
		std::string key = btnXml->Attribute("setting");
		std::cout << "\t\tAdding button for setting \"" << key << "\"...\n";
		//Map each button to its setting
		mButtons.emplace(make_pair(key, gui::TextButton(btnXml->IntAttribute("x"), btnXml->IntAttribute("y"), btnXml)));
	}

	std::cout << "\tParsing Labels...\n";
	for (auto lblXml = xml->FirstChildElement("Label");
		nullptr != lblXml;
		lblXml = lblXml->NextSiblingElement("Label"))
	{
		printf("\t\tAdding label: \"%s\"\n", lblXml->GetText());
		auto label = sf::Text(lblXml->GetText(), ResourceManager<sf::Font>::instance()->get(Constants::Strings::getMainFontPath()));
		auto bounds = label.getLocalBounds();
		label.setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);

		label.setPosition(lblXml->FloatAttribute("x"), lblXml->FloatAttribute("y"));

		label.setColor(Constants::Misc::getSecondaryTextColor());

		mLabels.push_back(std::move(label));
	}
	std::cout << "Done creating settings menu." << std::endl;
}

SettingsMenu::~SettingsMenu() {}

bool SettingsMenu::handleEvent(sf::Event& Event) {
	bool handled = true;
	switch (Event.type) {

		case sf::Event::MouseButtonPressed:
			updateAll(Event.mouseButton.x, Event.mouseButton.y);

			if (mBackButton.checkClick()) {
				//Prompt to save settings if they have changed.
				if (mSettingsChanged) {
					SceneManager::instance()->showDialogueBox(&mSaveChangesDialogueBox);
				}
				else {
					SceneManager::instance()->navigateToScene("MainMenu");
				}
			}
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

void SettingsMenu::update(sf::Time const& elapsedTime) {
	if (!mSaveChangesDialogueBox.resultProcessed())
	{
		switch (mSaveChangesDialogueBox.getResult())
		{
		case gui::DialogueBox::YES:
			Settings::save();
			SceneManager::instance()->navigateToScene("MainMenu");
			break;

		case gui::DialogueBox::NO:
			SceneManager::instance()->navigateToScene("MainMenu");
			break;

		case gui::DialogueBox::CANCEL:
			//Do nothing.
		default:
			break;
		}
	}
}

void SettingsMenu::draw(sf::RenderWindow& w) {
	w.draw(mBackdrop);

	w.draw(mBackButton);

	for (auto& entry : mSliders) {
		w.draw(entry.second);
	}


	for (auto& entry : mButtons) {
		w.draw(entry.second);
	}


	for (auto const& lbl : mLabels)
	{
		w.draw(lbl);
	}
}

void SettingsMenu::cleanup() {}

void SettingsMenu::updateAll(const int mouseX, const int mouseY) {
	const sf::Vector2i mousePos(mouseX, mouseY);

	mBackButton.update(mousePos);

	for (auto& entry : mSliders) {
		if (entry.second.update(mouseX, mouseY)) {
			//Update setting if slider value changed.
			Settings::set(entry.first, entry.second.getAbsoluteValue());
			mSettingsChanged = true;
		}
	}

	for (auto& entry : mButtons) {
		entry.second.update(mousePos);
		//TODO: apply change to setting
	}
}
