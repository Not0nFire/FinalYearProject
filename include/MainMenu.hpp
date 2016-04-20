#ifndef MAIN_MENU_SCENE_HPP
#define MAIN_MENU_SCENE_HPP

#include "Scene.hpp"
#include "Gui/Button.hpp"
#include "Gui/DialogueBox.hpp"

class SceneManager;

/*!
\class MainMenu
\brief First menu shown to the user.
*/
class MainMenu : public I_Scene {
public:
	/*!
	\brief Constructs a menu from xml.
	\param root Xml element, <Menu>, that contains button definitions and all other menu data.
	*/
	MainMenu(tinyxml2::XMLElement* root);
	virtual ~MainMenu();

	//! Checks to see if buttons are being hovered over or clicked.
	bool handleEvent(sf::Event& Event) override;

	//! Does nothing in this scene.
	void update(sf::Time const& elapsedTime) override;

	//! Draws background and buttons.
	void draw(sf::RenderWindow &w) override;

	//! Stops the background music.
	void cleanup() override;
private:
	gui::DialogueBox mQuitConfirmDialogue;

	//! Background sprite
	sf::Sprite mBackground;

	//! Background music that will play in this scene
	sf::Music mMusic;

	std::unique_ptr<gui::Button> mStartButton, mCreditsButton, mOptionsButton, mQuitButton;
};
#include "SceneManager.hpp"
#endif