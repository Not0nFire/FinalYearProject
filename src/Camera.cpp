#include <include\Camera.hpp>

Camera::Camera(sf::Vector2u screenSize) : 
View(sf::Vector2f(screenSize.x / 2, screenSize.y / 2), sf::Vector2f(screenSize)),
mScreenSize(screenSize)
{

}

Camera::Camera(sf::Vector2u screenSize, Actor * target) :
View(sf::Vector2f(screenSize.x / 2, screenSize.y / 2), sf::Vector2f(screenSize)),
mScreenSize(screenSize),
mTarget(target)
{
}

void Camera::setTarget(Actor* target)
{
	mTarget = target;
}

Actor * Camera::getTarget() const {
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
	if (mTarget != nullptr)
	{
		setCenter(mTarget->getPosition());
	}
}
