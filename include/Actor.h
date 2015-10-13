#ifndef _ACTOR_H
#define _ACTOR_H

#include <SFML/Graphics.hpp>
#include <include/Collidable.h>

class Actor : public sf::Sprite, public Collidable {
protected:
	bool mVisible;
public:
	Actor();
	Actor(sf::Texture &texture);
	Actor(const char* xml);	//todo: #include tinyXML
	Actor(const char* xml, sf::Texture &overrideTexture);
	virtual ~Actor();

	void setVisible(bool isVisible = true);
	bool toggleVisible();
	bool getVisible() const;

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states);
};
#endif