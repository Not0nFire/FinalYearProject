#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SFML/Audio/Listener.hpp>
#include <SFML/Graphics/View.hpp>
#include <include/Actor.hpp>

/*!
\brief Follow camera that can target an Actor
*/
class Camera : public sf::View
{
public:
	Camera(sf::Vector2u const &screenSize, sf::Vector2f const &boundingArea);
	Camera(sf::Vector2u const &screenSize, sf::Vector2f const &boundingArea, std::shared_ptr<Actor> const &target);

	void setTarget(std::shared_ptr<Actor> target);
	std::shared_ptr<Actor> getTarget() const;
	void clearTarget();

	/*!
	 * \brief Gets the top left corner of the camera.
	 * \returns The top left corner of the camera in world space.
	 */
	sf::Vector2f getDisplacement() const;

	/*!
	* \brief Updates camera's center to where the target is.
	* Clamps camera edges to stay inside of bounding area.
	* \see clamp()
	*/
	void update();
private:
	std::shared_ptr<Actor> mTarget;
	sf::Vector2f mScreenSize; //!< Keep a record of the screen size for resets
	sf::Vector2f mBoundingArea;	//!< Edges of camera cannot go beyond this area (starting at 0,0) regardless of target position

	//! \brief Clamps a vector2f to a min and max value
	static void clamp(sf::Vector2f& value, sf::Vector2f const &min, sf::Vector2f const &max);
};
#endif