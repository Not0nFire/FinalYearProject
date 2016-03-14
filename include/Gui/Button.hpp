#ifndef GUI_BUTTON_HPP
#define GUI_BUTTON_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <include/ResourceManager.hpp>
#include <include/TinyXML2/tinyxml2.h>

namespace gui
{
	/*!
	\class Button
	\brief Clickable sprite.
	*/
	class Button {
	public:
		Button(int x, int y, const tinyxml2::XMLElement* xmlButtonDefinition );
		virtual ~Button();

		void update(sf::Vector2i const& mousePos);
		bool checkClick() const;

		void draw(sf::RenderTarget &target) const;

		void enable();
		void disable();

	protected:
		enum State {
			NORMAL,
			HOVER,
			DISABLED
		} mState;

		void setState(State newState);

		sf::Sprite mSprite;

		sf::Sound mOnHoverSound;
		sf::IntRect mNormalSrcRect, mHoverSrcRect, mDisabledSrcRect;
	};
}
#endif