#include "include/Gui/Button.hpp"
#include <include/Settings.hpp>

using namespace gui;

Button::Button(int x, int y, const tinyxml2::XMLElement* xmlButtonDefinition) :
mGraphic()
{
	//get the texture
	std::string texturePath = xmlButtonDefinition->FirstChildElement("Texture")->Attribute("path");
	mGraphic.setTexture(&ResourceManager<sf::Texture>::instance()->get(texturePath));

	//get the hover sound (played when the mouse rolls over the button)
	std::string soundPath = xmlButtonDefinition->FirstChildElement("HoverSound")->Attribute("path");
	mOnHoverSound.setBuffer(ResourceManager<sf::SoundBuffer>::instance()->get(soundPath));

	//get the button dimensions
	auto dimensions = xmlButtonDefinition->FirstChildElement("Dimensions");
	int width = dimensions->IntAttribute("width");
	int height = dimensions->IntAttribute("height");

	mGraphic.setSize(sf::Vector2f(width, height));

	auto bounds = mGraphic.getLocalBounds();
	mGraphic.setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);

	mNormalSrcRect.width = width;
	mHoverSrcRect.width = width;
	mDisabledSrcRect.width = width;

	mNormalSrcRect.height = height;
	mHoverSrcRect.height = height;
	mDisabledSrcRect.height = height;

	//get the position of the normal state subimage
	auto normalImagePos = xmlButtonDefinition->FirstChildElement("NormalImage");
	mNormalSrcRect.left = normalImagePos->IntAttribute("x");
	mNormalSrcRect.top = normalImagePos->IntAttribute("y");

	//get the position of the hover state subimage
	auto hoverImagePos = xmlButtonDefinition->FirstChildElement("HoverImage");
	mHoverSrcRect.left = normalImagePos->IntAttribute("x");
	mHoverSrcRect.top = hoverImagePos->IntAttribute("y");

	auto disabledImagePos = xmlButtonDefinition->FirstChildElement("DisabledImage");
	mDisabledSrcRect.left = disabledImagePos->IntAttribute("x");
	mDisabledSrcRect.top = disabledImagePos->IntAttribute("y");

	//Parse the optional <Scale> tag and apply it to our sprite
	auto scale = xmlButtonDefinition->FirstChildElement("Scale");
	if (nullptr != scale) {
		float scaleX = scale->FloatAttribute("x");
		float scaleY = scale->FloatAttribute("y");

		mGraphic.setScale(scaleX, scaleY);
	}

	mGraphic.setPosition(x, y);

	auto name = xmlButtonDefinition->Attribute("name");
	if (nullptr != name)
	{
		mName = name;
	}

	mState = NORMAL;
	mGraphic.setTextureRect(mNormalSrcRect);
}

Button::~Button() {
}

void Button::update(sf::Vector2i const& mousePos) {
	mMouseOver = mGraphic.getGlobalBounds().contains(mousePos.x, mousePos.y);
	if (mState != DISABLED) {
		//if the sprite bounds contain the mouse coordinates...
		if (mMouseOver) {
			setState(HOVER);
		}
		else {
			setState(NORMAL);
		}
	}
}

bool Button::checkClick() const {
	return mState == HOVER;
}

void Button::enable() {
	if (mState == DISABLED) {
		setState(NORMAL);
	}
}

void Button::disable() {
	if (mState != DISABLED) {
		setState(DISABLED);
	}
}

bool Button::containsMouse() const {
	return mMouseOver;
}

std::string const& Button::getName() const {
	return mName;
}

void Button::setPosition(sf::Vector2f const& position) {
	mGraphic.setPosition(position);
}

sf::Vector2f const& Button::getPosition() const {
	return mGraphic.getPosition();
}

sf::Vector2f const& Button::getSize() const {
	return mGraphic.getSize();
}

sf::Vector2f const& Button::getScale() const {
	return mGraphic.getScale();
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mGraphic);
}

void Button::setState(State newState) {
	if (mState != newState) {
		mState = newState;

		switch (mState) {

			case NORMAL:
				mGraphic.setTextureRect(mNormalSrcRect);
				break;

			case HOVER:
				mGraphic.setTextureRect(mHoverSrcRect);
				mOnHoverSound.setVolume(Settings::getInt("EffectsVolume"));
				mOnHoverSound.play();
				break;

			case DISABLED:
				mGraphic.setTextureRect(mDisabledSrcRect);
				break;

			default:
				break;
		}//end switch
	}//end if
}
