#ifndef CURSOR_H
#define CURSOR_H

#include "ResourceManager.hpp"
#include <SFML/Graphics.hpp>

class Cursor {
public:
	static void setTexture(std::string const& texturePath);

	static void setPosition(int x, int y);

	static void draw(sf::RenderWindow &window);
private:
	Cursor();
	~Cursor();

	static sf::Sprite mSprite;
};
#endif