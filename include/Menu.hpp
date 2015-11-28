#ifndef _MENU_H
#define _MENU_H


#include <SFML/Graphics.hpp>
#include <include/SceneManager.hpp>
#include <include/MenuFunctions.h>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <vector>

class Menu : public I_Scene{
private:

	sfg::Desktop mDesktop;
	sfg::Box::Ptr mBox;

public:
	Menu(sfg::Box::Orientation orientation = sfg::Box::Orientation::VERTICAL, float spacing = 0.0f);

	static Menu* fromXML(const char* path);

	//Make these private when tinyXML is implemented
	void addButton(sf::String const &label, std::function<void()> onClick, sf::Vector2f const &minSize = sf::Vector2f());
	void addLabel(sf::String const &label, sf::Vector2f const &minSize = sf::Vector2f());

	void I_Scene::draw(sf::RenderWindow &w) override;
	void I_Scene::update(sf::Time const &elapsedTime) override;
	bool I_Scene::handleEvent(sf::Event &Event) override;
};
#endif