#ifndef _ACTOR_H
#define _ACTOR_H

#include <SFML/Graphics.hpp>
#include <include/Collision/Collidable.hpp>
#include <boost/thread/mutex.hpp>
#include <include/TinyXML2/tinyxml2.h>
#include <include/ResourceManager.hpp>
#include <Thor/Animations.hpp>

/*!
\brief Class for anything that animates and has a position.
Animates and has position and collision.
A lot is yet to be implemented. Will make heavy used of the thor::Animator class on itself.
\remarks Used for things like Pawns, Minions, Towers, etc...
*/
class Actor : public sf::Sprite, public collision::Collidable {
private:
	thor::Animator<sf::Sprite, std::string> mAnimator;	//!< Used to animate ourself
protected:
	bool mVisible;
	boost::mutex mMutex;	//!< Not to be confused with the private mutex in Collidable
public:
	/*!
	\param texture Spritesheet to be used for animation
	\param collisionMask Shape of actor used in collision checks
	\param maskOffset Position of the collisionMask relative to Actor position
	*/
	Actor(sf::Texture &texture, sf::Shape* collisionMask, sf::Vector2f const &maskOffset);
	explicit Actor(tinyxml2::XMLElement* xml);
	virtual ~Actor();

	void animate(sf::Time const &elapsedTime);
	void playAnimation(std::string name, bool loop = false);

	void setVisible(bool isVisible = true);
	bool toggleVisible();
	bool getVisible() const;
};
#endif