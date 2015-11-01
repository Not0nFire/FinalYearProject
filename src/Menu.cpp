#include "../include/Menu.hpp"

Menu::Menu(String const &text, Font const &font, void (*function)(String), Vector2f position, Color mainColor, Color hiliteColor, int item_spacing)
	: m_itemSpacing( item_spacing ), m_font( font ), m_mainColor( mainColor ), m_hiliteColor( hiliteColor ) {

	//push first item onto list with its associated function (pointer) and initialise selected iterator
	m_items.push_back( std::make_pair( Text( text, font ), function) );
	m_selected = m_items.begin();

	m_selected->first.setPosition( position );
	m_selected->first.setColor( hiliteColor );
}

void Menu::addItem(String const &text, void (*function)(String) ) {
	m_items.push_back( std::make_pair( Text( text, m_font ), function ) );

	Text* thisItem = &m_items.rbegin()->first;
	Text* lastItem = &( ++m_items.rbegin() )->first;
	
	//place this item below the last item
	thisItem->setPosition( lastItem->getPosition().x, lastItem->getPosition().y + lastItem->getCharacterSize() + m_itemSpacing);

	m_selected = m_items.begin();
}


//std::pair<Text, void (*)(void)> Menu::getItemAtIndex(unsigned int const index) const {
//	return m_items.at(index);
//}

void Menu::moveDown() {
	m_selected->first.setColor( m_mainColor );

	//move to top of list if trying to move past bottom
	if( ++m_selected == m_items.end() )
		m_selected = m_items.begin();

	m_selected->first.setColor( m_hiliteColor );
}

void Menu::moveUp() {
	m_selected->first.setColor( m_mainColor );

	//move to bottom of list if trying to move past top
	if ( m_selected == m_items.begin() )
		m_selected = m_items.end();

	m_selected--;

	m_selected->first.setColor( m_hiliteColor );
}

bool Menu::handleEvent(Event &Event) {

	bool handled = false;
	if (Event.type == Event::EventType::KeyPressed) {
		switch (Event.key.code) {
		case Keyboard::Down:
			moveDown();
			handled = true;
			break;
		case Keyboard::Up:
			moveUp();
			handled = true;
			break;
		case Keyboard::Return:
		case Keyboard::Space:
			select();
			handled = true;
			break;
		}
	}
	return handled;
}


//invokes the function pointer of the currently selected item
void Menu::select() {
	m_selected->second( m_selected->first.getString() );
}

//std::pair<Text, void (*)(String)> Menu::getSelected() const {
//	return *m_selected;
//}

void Menu::draw( RenderWindow &w ) {
	for( auto itr = m_items.begin();
		itr != m_items.end();
		itr++ )
	{
		w.draw( itr->first );
	}
}

//Vector2i Menu::getPosition() const {
//	return m_position;
//}

Color Menu::getMainColor() const {
	return m_mainColor;
}

Color Menu::getHiliteColor() const {
	return m_hiliteColor;
}

//void Menu::setPosition( Vector2i const &newPos ) {
//	m_position = newPos;
//}

void Menu::setMainColor( Color const &color ) {
	m_mainColor = color;
}

void Menu::setHiliteColor( Color const &color) {
	m_hiliteColor = color;
}