#ifndef _CAMERA_H
#define _CAMERA_H

#include <SFML/Graphics/View.hpp>
#include <include/Actor.hpp>

/*!
* Follow camera that can target an Actor
*/

class Camera : public sf::View
{
public:
	Camera(sf::Vector2u screenSize);
	Camera(sf::Vector2u screenSize, Actor* target);

	void setTarget(Actor* target);
	Actor* getTarget() const;
	void clearTarget();

	sf::Vector2f getDisplacement() const;

	/*!
	* Updates camera's center to where the target is
	*/
	void update();
private:
	Actor* mTarget;
	sf::Vector2f mScreenSize; //!< Keep a record of the screen size for resets
};
#endif