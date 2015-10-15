#ifndef _ACTOR_H
#define _ACTOR_H

#include <SFML/Graphics.hpp>
#include <include/Collidable.h>

//We can use this class for anything that animates and has collision: hero. minions, buttons, effects, abilities, etc...
class Actor : public sf::Sprite, public Collidable {
protected:
	bool mVisible;
	boost::mutex mMutex;	//Not to be confused with the private mutex in Collidable
public:
	Actor();
	Actor(sf::Texture &texture);
	Actor(const char* xml);	//todo: #include tinyXML
	Actor(const char* xml, sf::Texture &overrideTexture);
	virtual ~Actor();

	void setVisible(bool isVisible = true);
	bool toggleVisible();
	bool getVisible() const;
};
#endif