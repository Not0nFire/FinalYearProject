#include <include/Gui/Slider.hpp>

gui::Slider::Slider(tinyxml2::XMLElement* xml) :
mMinValue(xml->IntAttribute("minValue")),
mMaxValue(xml->IntAttribute("maxValue")),
mValue(0.f),
mBlip(0, 0, xml->FirstChildElement("Blip")),
mBar(sf::Vector2f(xml->FloatAttribute("width"), xml->FloatAttribute("height")))
{
	auto bounds = mBar.getLocalBounds();
	mBar.setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);

	auto startingValueXml = xml->FirstChildElement("StartingValue");
	if (nullptr != startingValueXml) {
		startingValueXml->QueryFloatText(&mValue);
	}

	std::string barTexPath = xml->FirstChildElement("BarTexture")->Attribute("path");
	mBar.setTexture(&ResourceManager<sf::Texture>::instance()->get(barTexPath));

	mBlip.setPosition(sf::Vector2f(bounds.left + (mValue * bounds.width), mBar.getPosition().y));

	//auto cap = xml->IntAttribute("capWidth");
	//mBlipCapLeft = bounds.left + cap;
	//mBlipCapRight = bounds.left + bounds.width - cap;
}

gui::Slider::~Slider() {}

void gui::Slider::update(int mouseX, int mouseY) {
	const bool mouseDown = sf::Mouse::isButtonPressed(sf::Mouse::Left);

	mBlip.update({ mouseX, mouseY });

	auto bounds = mBar.getGlobalBounds();
	if (mouseDown && bounds.contains(mouseX, mouseY)) {
		//Value is a float between 0.0 and 1.0, representing how far along the bar the blip is.
		mValue = (mouseX - bounds.left) / bounds.width;

		//auto newBlipPos = sf::Vector2f(bounds.left + (mValue * bounds.width), mBlip.getPosition().y);
		//if (newBlipPos.x > mBlipCapRight)
		//{
		//	newBlipPos.x = mBlipCapRight;
		//}
		//else if (newBlipPos.x < mBlipCapLeft)
		//{
		//	newBlipPos.x = mBlipCapLeft;
		//}

		mBlip.setPosition(sf::Vector2f(bounds.left + (mValue * bounds.width), mBlip.getPosition().y));
	}
}

void gui::Slider::setValue(const int value) {
	if (value > mMaxValue) {
		mValue = 1.f;
	}
	else if (value < mMinValue) {
		mValue = 0.f;
	}
	else {
		mValue = value / (mMaxValue - mMinValue);
	}
}

int gui::Slider::getAbsoluteValue() const {
	return std::max(mMinValue, std::min(mMaxValue, static_cast<int>(mValue * (mMaxValue - mMinValue) + mMinValue)));
}

void gui::Slider::setPosition(sf::Vector2f const& position) {
	mBar.setPosition(position);
	auto bounds = mBar.getGlobalBounds();
	mBlip.setPosition(sf::Vector2f(bounds.left + (mValue * bounds.width), mBar.getPosition().y));
}

sf::Vector2f const& gui::Slider::getPosition() const {
	return mBar.getPosition();
}

float gui::Slider::getNormalisedValue() const {
	return mValue;
}

void gui::Slider::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mBar, states);
	target.draw(mBlip, states);
}
