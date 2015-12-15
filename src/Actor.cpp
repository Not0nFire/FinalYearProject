#include <include\Actor.hpp>

Actor::Actor(sf::Texture &texture, sf::Shape* collisionMask, sf::Vector2f const &maskOffset)
	: sf::Sprite(texture), Collidable(collisionMask, maskOffset), mVisible(true) {
	setOrigin(texture.getSize().x * 0.5f, texture.getSize().y * 0.5f);
}

//Actor::Actor(const char* xml) {
//
//}
//
//Actor::Actor(const char* xml, sf::Texture &overrideTexture) {
//
//}

Actor::~Actor() {

}

//void Actor::animate(sf::Time const &elapsedTime) {
//	mAnimator.update(elapsedTime);
//	mAnimator.animate(*this);
//}
//
//bool Actor::addAnimation(std::string name, thor::FrameAnimation &anim, sf::Time const &duration) {
//	mAnimator.addAnimation(name, anim, duration);
//}
//void Actor::playAnimation(std::string name, bool loop = false) {
//	mAnimator.playAnimation(name, loop);
//}
//void Actor::stopAnimation() {
//	mAnimator.stopAnimation();
//}
//bool Actor::isPlayingAnimation() const {
//	return mAnimator.isPlayingAnimation();
//}
//std::string Actor::getPlayingAnimation() const {
//	return mAnimator.getPlayingAnimation();
//}

void Actor::setVisible(bool isVisible) {
	mVisible = isVisible;
}
bool Actor::toggleVisible() {
	mVisible = !mVisible;
	return mVisible;
}
bool Actor::getVisible() const {
	return mVisible;
}