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

	static Camera const& getDefaultCamera();

	void setTarget(std::shared_ptr<Actor> target);
	std::weak_ptr<Actor> getTarget() const;
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
	void update(float deltaSeconds);

	/*!
	\brief Locks the camera to its current position or target.
	Does nothing if already locked.
	*/
	void lock();

	/*!
	\brief Unlocks the camera, allowing the player to move it.
	Does nothing if already unlocked.
	*/
	void unlock();

	void toggleLock();

	void setBoundingArea(sf::Vector2f const& bounds);

	/*!
	\brief Moves the camera if the mouse is close to the screen edge.
	Assumes that the mouse position is NOT translated from screen to game coordinates.
	\param mousePos The position of the mouse
	*/
	void doMouseMove(sf::Vector2f mousePos);

	sf::Vector2f screenPositionToGamePosition(const int x, const int y) const;
private:
	std::weak_ptr<Actor> mTarget;
	sf::Vector2f mBoundingArea;	//!< Edges of camera cannot go beyond this area (starting at 0,0) regardless of target position

	bool mLocked;	//!< True if the camera cannot be moved by the player.
	const float mEdgeMoveThreshold;	//!< How close the mouse needs to be to the edge of the viewport before the camera moves.
	const float mMoveSpeed;
	sf::Vector2f mMoveDirection;

	//! \brief Clamps a vector2f to a min and max value
	static void clamp(sf::Vector2f& value, sf::Vector2f const &min, sf::Vector2f const &max);

	//! Used to convert a point from screen coords to game coords.
	sf::Vector2f mScalingFactor;

	void calculateScalingFactor();

	/*!
	\brief Sets the center of the view, accounting for bounds.
	Also sets sf::Listener position.
	*/
	void setClampedCenter(sf::Vector2f newCenter);
};
#endif