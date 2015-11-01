#include <include\Actor.hpp>

Actor::Actor(sf::Texture &texture)
	: sf::Sprite(texture), Collidable(new sf::CircleShape(20, 8), sf::Vector2f(0.0f, texture.getSize().y * 0.6f)), mVisible(true) {
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