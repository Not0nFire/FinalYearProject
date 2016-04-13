#include <include\Camera.hpp>
#include <include/Constants.h>

Camera::Camera(sf::Vector2f const& viewportSize, sf::Vector2f const &boundingArea) :
View(viewportSize * 0.5f, viewportSize),
mBoundingArea(boundingArea)
{
	_ASSERT(viewportSize.x <= boundingArea.x);
	_ASSERT(viewportSize.y <= boundingArea.y);

	sf::Listener::setDirection(0.f, -1.f, 0.f);
}

Camera::Camera(sf::Vector2f const& viewportSize, sf::Vector2f const &boundingArea, std::shared_ptr<Actor> const &target) :
View(viewportSize * 0.5f, viewportSize),
mTarget(target),
mBoundingArea(boundingArea)
{
	_ASSERT(viewportSize.x <= boundingArea.x);
	_ASSERT(viewportSize.y <= boundingArea.y);

	sf::Listener::setDirection(0.f, -1.f, 0.f);
}

void Camera::setTarget(std::shared_ptr<Actor> target)
{
	mTarget = target;
}

std::shared_ptr<Actor> Camera::getTarget() const {
	return mTarget;
}

void Camera::clearTarget()
{
	mTarget = nullptr;
}

sf::Vector2f Camera::getDisplacement() const {
	//return the point at the top left corner
	return getCenter() - (getSize() * 0.5f);
}

void Camera::update()
{
	
	if (mTarget)
	{
		sf::Vector2f newCenter = mTarget->getPosition();

		//set listener position before clamping
		sf::Listener::setPosition(sf::Vector3f(newCenter.x, newCenter.y, 0.f));

		//stop edge of camera going out of bounds
		const sf::Vector2f halfSize = getSize() * 0.5f;
		clamp(newCenter, halfSize, mBoundingArea - halfSize);

		setCenter(newCenter);
	}
}

sf::Vector2f Camera::mousePositionToGamePosition(const int x, const int y) const {
	//Taken from sf::RenderTarget::mapPixelToCoord()
	const auto& viewport = getViewport();
	const auto& screenSize = Constants::Vectors::getScreenSize();
	sf::IntRect adjustedViewport(
		static_cast<int>(0.5f + screenSize.x  * viewport.left),
		static_cast<int>(0.5f + screenSize.y * viewport.top),
		static_cast<int>(0.5f + screenSize.x  * viewport.width),
		static_cast<int>(0.5f + screenSize.y * viewport.height)
	);

	sf::Vector2f normalized;
	normalized.x = -1.f + 2.f * (x - adjustedViewport.left) / adjustedViewport.width;
	normalized.y = 1.f - 2.f * (y - adjustedViewport.top) / adjustedViewport.height;

	//const auto debug = getInverseTransform().transformPoint(normalized);
	//printf("Mouse : %f, %f\n", debug.x, debug.y);

	return getInverseTransform().transformPoint(normalized);
}

void Camera::clamp(sf::Vector2f& value, sf::Vector2f const& min, sf::Vector2f const& max) {
	//assert that min is less than max
	_ASSERT(min.x <= max.x && min.y <= max.y);

	//clamp x value
	if (value.x < min.x) {
		value.x = min.x;
	}
	else if (value.x > max.x) {
		value.x = max.x;
	}

	//clamp y value
	if (value.y < min.y) {
		value.y = min.y;
	}
	else if (value.y > max.y) {
		value.y = max.y;
	}
}
