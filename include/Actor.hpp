#ifndef _ACTOR_H
#define _ACTOR_H

#include <SFML/Graphics.hpp>
#include <include/Collision/Collidable.hpp>
#include <include/TinyXML2/tinyxml2.h>
#include <include/ResourceManager.hpp>
#include <Thor/Animations.hpp>
#include <iostream>

/*!
\brief Class for anything that animates and has a position.
Animates and has position and collision.
\remarks Used for things like Pawns, Minions, Towers, etc...
*/
class Actor : public sf::Sprite, public collision::Collidable {
private:
	thor::Animator<sf::Sprite, std::string> mAnimator;	//!< Used to animate ourself
protected:
	bool mVisible;
public:
	/*!
	\param texture Spritesheet to be used for animation
	\param collisionMask Shape of actor used in collision checks
	\param maskOffset Position of the collisionMask relative to Actor position
	*/
	Actor(sf::Texture &texture, sf::Shape* collisionMask, sf::Vector2f const &maskOffset);
	explicit Actor(tinyxml2::XMLElement* xml);
	virtual ~Actor();

	//! Updates the animator and applies it to ourself
	void animate(sf::Time const &elapsedTime);
	bool isPlayingAnimation() const;	//!< Returns true if an animation is currently playing.
	std::string getPlayingAnimation() const;	//!< Gets the name of the playing animation. Undefined behaviour if no animation playing.
	void playAnimation(std::string const &name, bool loop = false);	//!< Begins playing an animation by name.

	void setVisible(bool isVisible = true);
	bool toggleVisible();
	bool getVisible() const;

	virtual void draw(sf::RenderTarget &target) const;
};
#endif