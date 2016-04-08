#include <include\Actor.hpp>

Actor::Actor(sf::Texture &texture, sf::Shape* collisionMask, sf::Vector2f const &maskOffset)
	: sf::Sprite(texture), Collidable(collisionMask, maskOffset), mVisible(true)
{
	setOrigin(texture.getSize().x * 0.5f, texture.getSize().y * 0.5f);
}

Actor::Actor(tinyxml2::XMLElement* xml) :
//Get texture path from xml and request it from resource manager
Sprite(ResourceManager<sf::Texture>::instance()->get(xml->FirstChildElement("Texture")->GetText())),
Collidable(xml->FirstChildElement("Collidable")),
mAnimator(),
mVisible(true)
{
	//Assert that xml tag name is Actor
	_ASSERT(std::string(xml->Name()) == "Actor");

	using namespace thor;

	//Read the origin from xml, or set to centre of sprite if not found
	tinyxml2::XMLElement* originElement = xml->FirstChildElement("Origin");
	if (originElement)
	{
		setOrigin(atof(originElement->Attribute("x")), atof(originElement->Attribute("y")));
	}
	else
	{
		setOrigin(getLocalBounds().width * 0.5f, getLocalBounds().height * 0.5f);
	}

	//For each <Animation> tag
	for (tinyxml2::XMLElement* animElement = xml->FirstChildElement("Animation");
		animElement != nullptr;
		animElement = animElement->NextSiblingElement("Animation"))
	{
		std::string path = animElement->Attribute("path");	//Path to file containing definition of animation
		std::string name = animElement->FirstChildElement("Name")->GetText();	//Name of animation, as it will be known locally
		float duration = atof(animElement->FirstChildElement("Duration")->GetText());	//Length of animation in seconds
		
		//Create a reference to the animation that we get from the ResourceManager
		RefAnimation<FrameAnimation> animation = RefAnimation<FrameAnimation>(
			ResourceManager<FrameAnimation>::instance()->get(path)
			);

		//Add the animation to our animator
		mAnimator.addAnimation(name, animation, sf::seconds(duration));
	}
}

Actor::~Actor() {

}

void Actor::animate(sf::Time const &elapsedTime) {
	mAnimator.update(elapsedTime);
	mAnimator.animate(*this);
}

//bool Actor::addAnimation(std::string name, thor::FrameAnimation &anim, sf::Time const &duration) {
//	mAnimator.addAnimation(name, anim, duration);
//}
void Actor::playAnimation(std::string const &name, bool loop) {
	mAnimator.playAnimation(name, loop);
}
//void Actor::stopAnimation() {
//	mAnimator.stopAnimation();
//}
bool Actor::isPlayingAnimation() const {
	return mAnimator.isPlayingAnimation();
}
std::string Actor::getPlayingAnimation() const {
	return mAnimator.getPlayingAnimation();
}

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

void Actor::draw(sf::RenderTarget& target) const {
	target.draw(*this);
}
