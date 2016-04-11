#ifndef GUI_DIALOGUE_BOX_HPP
#define GUI_DIALOGUE_BOX_HPP

#include <SFML/Graphics.hpp>
#include "TextButton.hpp"

namespace gui
{
	class DialogueBox : public sf::Drawable {
	public:
		DialogueBox(sf::Vector2f const& centre, sf::Vector2f const& size, std::string const& title, std::string const& message, std::string const& yesText = "Yes", std::string const& noText = "No", unsigned int titleCharSize = 40u, unsigned int msgCharSize = 20u);
		~DialogueBox();

		void open();
		bool isOpen() const;

		bool handleEvent(sf::Event const& theEvent);

		enum Result {
			YES,
			NO,
			CANCEL
		};

		Result getResult() const;

	private:
		Result mResult;
		sf::Text mTitle, mMessage;
		std::map<Result, std::unique_ptr<Button>> mButtonMap;
		bool mIsOpen;
		sf::RectangleShape mBoxGraphic;

		void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

		void updateButtons(sf::Vector2i const& mousePosition);

		void updateResult();
	};
}
#endif