#ifndef _MENU_H
#define _MENU_H


#include "SFML\Graphics.hpp"
#include "SceneManager.h"
#include <vector>

using namespace sf;

//Vertical menu class. Does not currently support horizontal arbitrary layouts.
class Menu : public I_Scene{
private:

	std::vector< std::pair<Text, void (*)(String)> > m_items;

	int m_itemSpacing;

	std::vector< std::pair<Text, void (*)(String)> >::iterator m_selected;

	Font m_font;

	//Vector2i m_position;

	Color m_mainColor, m_hiliteColor;

public:

	//creates a menu with one item
	Menu(String const &text, Font const &font, void (*function)(String), Vector2f position, Color mainColor = Color::White, Color hiliteColor = Color::Yellow, int item_Spacing = 10);

	//adds an item to the menu with a custom function which is called when the item is selected
	void addItem(String const &text, void (*function)(String));

	//std::pair<Text, void (*)(void)> getItemAtIndex(unsigned int const index) const;

	void moveUp();
	void moveDown();

	//invokes the function pointer of the currently selected item
	void select();

	void I_Scene::draw(RenderWindow &w);
	void I_Scene::update(Time const &elapsedTime) { /* do nothing */ }
	bool I_Scene::handleEvent(Event &Event);


	//std::pair< Text, void(*)(String) > getSelected() const;
	//Vector2i getPosition() const;
	Color getMainColor() const;
	Color getHiliteColor() const;

	//void setPosition( Vector2i const &newPos );
	void setMainColor( Color const &color );
	void setHiliteColor( Color const &color );
};
#endif