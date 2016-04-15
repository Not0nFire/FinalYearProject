#ifndef SETTTINGS_MENU_HPP
#define SETTINGS_MENU_HPP

#include "Scene.hpp"
#include "Gui/DialogueBox.hpp"
#include "Gui/Slider.hpp"
#include <list>

class SettingsMenu : public I_Scene {
public:
	SettingsMenu(tinyxml2::XMLElement* xml);
	virtual ~SettingsMenu();

	bool handleEvent(sf::Event& Event) override;
	void update(sf::Time const& elapsedTime) override;
	void draw(sf::RenderWindow& w) override;
	void cleanup() override;

private:
	gui::DialogueBox mSaveChangesDialogueBox;
	bool mSettingsChanged;

	const sf::Sprite mBackdrop;

	gui::Button mBackButton;

	//! Map of setting names to sliders
	std::map<std::string, gui::Slider> mSliders;

	//! Map of setting names to buttons
	std::map<std::string, gui::TextButton> mButtons;

	//! List of labels
	std::list<sf::Text> mLabels;

	void updateAll(const int mouseX, const int mouseY);
};
#endif