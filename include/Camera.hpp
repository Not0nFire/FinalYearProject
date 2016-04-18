#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SFML/Graphics/View.hpp>
#include <include/Actor.hpp>

/*!
\brief Follow camera that can target an Actor
*/
class Camera : public sf::View
{
public:
	Camera(sf::Vector2f const& viewportSize, sf::Vector2f const &boundingArea = sf::Vector2f(FLT_MAX, FLT_MAX));
	Camera(sf::Vector2f const& viewportSize, sf::Vector2f const &boundingArea, std::shared_ptr<Actor> const &target);

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

	sf::Vector2f screenPositionToGamePosition(const int x, const int y) const;
private:
	std::shared_ptr<Actor> mTarget;
	sf::Vector2f mBoundingArea;	//!< Edges of camera cannot go beyond this area (starting at 0,0) regardless of target position

	//! \brief Clamps a vector2f to a min and max value
	static void clamp(sf::Vector2f& value, sf::Vector2f const &min, sf::Vector2f const &max);

	//! Used to convert a point from screen coords to game coords.
	sf::Vector2f mScalingFactor;

	void calculateScalingFactor();
};
#endif