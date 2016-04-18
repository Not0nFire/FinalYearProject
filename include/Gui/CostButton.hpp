#ifndef GUI_COST_BUTTON_HPP
#define GUI_COST_BUTTON_HPP

#include "Button.hpp"
namespace gui {
	class CostButton : public Button {
	public:
		CostButton(int x, int y, tinyxml2::XMLElement* xmlDefinition, std::shared_ptr<const int> resource);
		~CostButton();

		void update(sf::Vector2i const& mousePos) override;
	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		const int mCost;
		sf::Text mCostText;
		std::shared_ptr<const int> mResource;
	};
}
#endif