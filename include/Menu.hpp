#ifndef _MENU_H
#define _MENU_H


#include <SFML/Graphics.hpp>
#include <include/SceneManager.hpp>
#include <include/ResourceManager.hpp>
#include <include/MenuFunctions.h>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <vector>
#include <SFML/Audio.hpp>

/*!
\brief Menu implemented using [SFGUI](http://sfgui.sfml-dev.de/)
*/
class Menu : public I_Scene{
private:

	sfg::Desktop mDesktop;
	sfg::Box::Ptr mBox;
	std::shared_ptr<sfg::SFGUI> mSFGUI;
	sf::Music mBgMusic;

	sf::Sprite mBackground;

public:
	Menu(std::shared_ptr<sfg::SFGUI> gui, std::string const &themePath = "./res/gui.theme", sfg::Box::Orientation orientation = sfg::Box::Orientation::VERTICAL, float spacing = 10.0f);

	static Menu* fromXML(const char* path);

	//Make these private when tinyXML is implemented
	void addButton(sf::String const &label, std::function<void()> onClick, sf::Vector2f const &minSize = sf::Vector2f());
	void addLabel(sf::String const &label, bool title = false, sf::Vector2f const &minSize = sf::Vector2f());

	void I_Scene::draw(sf::RenderWindow &w) override;
	void I_Scene::update(sf::Time const &elapsedTime) override;
	bool I_Scene::handleEvent(sf::Event &Event) override;
	void I_Scene::cleanup() override;
};
#endif