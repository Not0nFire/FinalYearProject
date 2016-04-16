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
	class Button : public sf::Drawable {
	public:
		Button(int x, int y, const tinyxml2::XMLElement* xmlButtonDefinition );
		virtual ~Button();

		virtual void update(sf::Vector2i const& mousePos);
		bool checkClick() const;

		void enable();	
		void disable();

		bool containsMouse() const;

		std::string const& getName() const;

		sf::Vector2f const& getPosition() const;

		sf::Vector2f const& getSize() const;

		sf::Vector2f const& getScale() const;

	protected:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		enum State {
			NORMAL,
			HOVER,
			DISABLED
		} mState;

		void setState(State newState);

	private:
		sf::RectangleShape mGraphic;

		sf::Sound mOnHoverSound;
		sf::IntRect mNormalSrcRect, mHoverSrcRect, mDisabledSrcRect;

		std::string mName;

		bool mMouseOver;	//!< True if the mouse is within bounds
	};
}
#endif