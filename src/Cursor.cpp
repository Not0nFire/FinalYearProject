#include "include/Cursor.h"

sf::Sprite Cursor::mSprite = sf::Sprite();

void Cursor::setTexture(std::string const& texturePath) {
	mSprite.setTexture(ResourceManager<sf::Texture>::instance()->get(texturePath));
}

void Cursor::setPosition(int x, int y) {
	mSprite.setPosition(x, y);
}

void Cursor::draw(sf::RenderWindow& window) {
	window.draw(mSprite);
}

Cursor::Cursor() {}

Cursor::~Cursor() {}
