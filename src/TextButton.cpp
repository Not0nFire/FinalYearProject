#include <include/Gui/TextButton.hpp>

using namespace gui;

TextButton::TextButton(int x, int y, tinyxml2::XMLElement* xmlDefinition) :
Button(x, y, xmlDefinition),
mText("", ResourceManager<sf::Font>::instance()->get("./res/fonts/KENVECTOR_FUTURE.TTF"))
{
	centreText();
	mText.setColor(sf::Color(73, 40, 17));
}

TextButton::TextButton(int x, int y, tinyxml2::XMLElement* xmlDefinition, std::string const& text, unsigned int characterSize) :
Button(x, y, xmlDefinition),
mText(text, ResourceManager<sf::Font>::instance()->get("./res/fonts/KENVECTOR_FUTURE.TTF"), characterSize)
{
	centreText();
	mText.setColor(sf::Color(73, 40, 17));
}

void TextButton::setText(std::string const& text) {
	mText.setString(text);
	centreText();
}

std::string const& TextButton::getText() const {
	return mText.getString();
}

void TextButton::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	Button::draw(target, states);
	target.draw(mText, states);
}

void TextButton::centreText() {
	auto txtBounds = mText.getGlobalBounds();
	mText.setOrigin(txtBounds.width * 0.5f, txtBounds.height * 1.f);
	mText.setPosition(getPosition());
}
