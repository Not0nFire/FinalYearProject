#ifndef GUI_TEXT_BUTTON_HPP
#define GUI_TEXT_BUTTON_HPP

#include "Button.hpp"

namespace gui
{
	class TextButton : public Button {
	public:
		TextButton(int x, int y, tinyxml2::XMLElement* xmlDefinition);
		TextButton(int x, int y, tinyxml2::XMLElement* xmlDefinition, std::string const& text, unsigned int characterSize = 20u);

		void setText(std::string const& text);
		std::string const& getText() const;

	protected:
		void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	private:
		sf::Text mText;

		void centreText();
	};
}
#endif