#include <include\Camera.hpp>
#include <include/Constants.h>
#include <include/Settings.hpp>
#include <SFML/Audio/Listener.hpp>
#include <Thor/Vectors.hpp>

Camera::Camera(sf::Vector2f const& viewportSize, sf::Vector2f const &boundingArea) :
View(viewportSize * 0.5f, viewportSize),
mBoundingArea(boundingArea),
mLocked(true),
mEdgeMoveThreshold(Constants::Numbers::getCameraEdgeMoveThreshold()),
mMoveSpeed(Constants::Numbers::getCameraMoveSpeed())
{
	_ASSERT(viewportSize.x <= boundingArea.x);
	_ASSERT(viewportSize.y <= boundingArea.y);

	sf::Listener::setDirection(0.f, -1.f, 0.f);

	calculateScalingFactor();
}

Camera::Camera(sf::Vector2f const& viewportSize, sf::Vector2f const &boundingArea, std::shared_ptr<Actor> const &target) :
View(viewportSize * 0.5f, viewportSize),
mTarget(target),
mBoundingArea(boundingArea),
mLocked(true),
mEdgeMoveThreshold(Constants::Numbers::getCameraEdgeMoveThreshold()),
mMoveSpeed(Constants::Numbers::getCameraMoveSpeed())
{
	_ASSERT(viewportSize.x <= boundingArea.x);
	_ASSERT(viewportSize.y <= boundingArea.y);

	sf::Listener::setDirection(0.f, -1.f, 0.f);

	calculateScalingFactor();
}

Camera const& Camera::getDefaultCamera() {
	static const auto defaultCamera = Camera(sf::Vector2f(Settings::getVector2i("Resolution").x, Settings::getVector2i("Resolution").y));
	return defaultCamera;
}

void Camera::setTarget(std::shared_ptr<Actor> target)
{
	mTarget = target;
}

std::weak_ptr<Actor> Camera::getTarget() const {
	return mTarget;
}

void Camera::clearTarget()
{
	mTarget.reset();
}

sf::Vector2f Camera::getDisplacement() const {
	//return the point at the top left corner
	return getCenter() - (getSize() * 0.5f);
}

void Camera::update(float deltaSeconds) {
	
	if (mLocked) {
		//Track our target...
		if (auto target = mTarget.lock())
		{
			auto const displacement = target->getPosition() - getCenter();

			setClampedCenter(getCenter() + (displacement * deltaSeconds));

			sf::Listener::setPosition(target->getPosition().x, target->getPosition().y, 0.f);
		}
	}
	//Handle free moving
	else {
		setClampedCenter(getCenter() + (mMoveDirection * mMoveSpeed * deltaSeconds));
	}
}

void Camera::lock() {
	if (!mLocked) {
		mLocked = true;
	}
}

void Camera::unlock() {
	if (mLocked) {
		mLocked = false;
	}
}

void Camera::toggleLock() {
	mLocked = !mLocked;
}

void Camera::doMouseMove(sf::Vector2f mousePos) {
	auto const& screenSize = Constants::Vectors::getScreenSize();

	if (!mLocked) {
		//Check if we should move horizontally...
		if (mousePos.x < mEdgeMoveThreshold) {
			mMoveDirection.x = -1.f;
		}
		else if (mousePos.x > screenSize.x - mEdgeMoveThreshold){
			mMoveDirection.x = 1.f;
		}
		else {
			mMoveDirection.x = 0.f;
		}

		//Check if we should move vertically...
		if (mousePos.y < mEdgeMoveThreshold) {
			mMoveDirection.y = -1.f;
		}
		else if (mousePos.y > screenSize.y - mEdgeMoveThreshold){
			mMoveDirection.y = 1.f;
		}
		else {
			mMoveDirection.y = 0.f;
		}
	}
}

sf::Vector2f Camera::screenPositionToGamePosition(const int x, const int y) const {
	return sf::Vector2f(x*mScalingFactor.x, y*mScalingFactor.y) + getDisplacement();
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

void Camera::calculateScalingFactor() {
	auto const& screenSize = Constants::Vectors::getScreenSize();
	auto const& gamesize = Settings::getVector2i("Resolution");

	mScalingFactor.x = static_cast<float>(gamesize.x) / static_cast<float>(screenSize.x);
	mScalingFactor.y = static_cast<float>(gamesize.y) / static_cast<float>(screenSize.y);
}

void Camera::setClampedCenter(sf::Vector2f newCenter) {
	//set listener position before clamping
	sf::Listener::setPosition(sf::Vector3f(newCenter.x, newCenter.y, 0.f));

	//stop edge of camera going out of bounds
	const sf::Vector2f halfSize = getSize() * 0.5f;
	clamp(newCenter, halfSize, mBoundingArea - halfSize);

	setCenter(newCenter);
}
