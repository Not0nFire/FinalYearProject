#include "include/Gui/Button.hpp"

using namespace gui;

Button::Button(int x, int y, const tinyxml2::XMLElement* xmlButtonDefinition) :
mSprite()
{
	//get the texture
	std::string texturePath = xmlButtonDefinition->FirstChildElement("Texture")->Attribute("path");
	mSprite.setTexture(ResourceManager<sf::Texture>::instance()->get(texturePath));

	//get the hover sound (played when the mouse rolls over the button)
	std::string soundPath = xmlButtonDefinition->FirstChildElement("HoverSound")->Attribute("path");
	mOnHoverSound.setBuffer(ResourceManager<sf::SoundBuffer>::instance()->get(soundPath));

	//get the button dimensions
	auto dimensions = xmlButtonDefinition->FirstChildElement("Dimensions");
	int width = atoi(dimensions->Attribute("width"));
	int height = atoi(dimensions->Attribute("height"));

	mNormalSrcRect.width = width;
	mHoverSrcRect.width = width;

	mNormalSrcRect.height = height;
	mHoverSrcRect.height = height;

	//get the position of the normal state subimage
	auto normalImagePos = xmlButtonDefinition->FirstChildElement("NormalImage");
	mNormalSrcRect.left = atoi(normalImagePos->Attribute("x"));
	mNormalSrcRect.top = atoi(normalImagePos->Attribute("y"));

	//get the position of the hover state subimage
	auto hoverImagePos = xmlButtonDefinition->FirstChildElement("HoverImage");
	mHoverSrcRect.left = atoi(normalImagePos->Attribute("x"));
	mHoverSrcRect.top = atoi(hoverImagePos->Attribute("y"));

	mSprite.setPosition(x, y);
	mSprite.setTextureRect(mNormalSrcRect);
	mState = NORMAL;
}

Button::~Button() {}

void Button::update(sf::Vector2i const& mousePos) {
	//if the sprite bounds contain the mouse coordinates...
	if (mSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
		setState(HOVER);
	} else {
		setState(NORMAL);
	}
}

bool Button::checkClick() const {
	return mState == HOVER;
}

void Button::draw(sf::RenderTarget& target) const {
	target.draw(mSprite);
}

void Button::setState(State newState) {
	if (mState != newState) {
		mState = newState;

		if (mState == NORMAL) {
			mSprite.setTextureRect(mNormalSrcRect);
		}
		else {	//mState == HOVER
			mSprite.setTextureRect(mHoverSrcRect);
			mOnHoverSound.play();
		}
	}
}
