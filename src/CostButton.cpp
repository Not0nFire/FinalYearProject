#include <include/Gui/CostButton.hpp>

using namespace gui;

CostButton::CostButton(int x, int y, tinyxml2::XMLElement* xmlDefinition, std::shared_ptr<const int> resource) :
Button(x,y,xmlDefinition),
mCost(xmlDefinition->IntAttribute("cost")),
mCostText(std::to_string(mCost), ResourceManager<sf::Font>::instance()->get("./res/fonts/NEVIS.TTF"), 15u),
mResource(resource)
{
	mCostText.setPosition(getPosition());
	
	auto offsetXml = xmlDefinition->FirstChildElement("CostLabelOffset");
	if (nullptr != offsetXml) {
		mCostText.move(offsetXml->FloatAttribute("x"), offsetXml->FloatAttribute("y"));
	}
}

CostButton::~CostButton() {}

void CostButton::update(sf::Vector2i const& mousePos) {
	if (mState == DISABLED && *mResource > mCost) {
		enable();
	}
	else if (*mResource < mCost) {
		disable();
	}

	Button::update(mousePos);
}

void CostButton::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	Button::draw(target, states);
	target.draw(mCostText, states);
}
