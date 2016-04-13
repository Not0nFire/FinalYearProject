#ifndef GUI_SLIDER_HPP
#define GUI_SLIDER_HPP

#include "Button.hpp"

namespace gui
{
	class Slider : public sf::Drawable {
	public:
		Slider(tinyxml2::XMLElement* xml);
		virtual ~Slider();

		void update(int mouseX, int mouseY);

		void setValue(const int value);
		int getAbsoluteValue() const;
		float getNormalisedValue() const;

		void setPosition(sf::Vector2f const& position);
		sf::Vector2f const& getPosition() const;
	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		//! Min and max values for the slider. Used when getValue() or setValue() is called.
		const int mMinValue, mMaxValue;

		//! From 0.0 to 1.0, how far along the bar the blip is.
		float mValue;

		//! Visual indicator of slider progress.
		Button mBlip;

		//! Visual representation of slider bar.
		sf::RectangleShape mBar;

		//! How far left/right the blip is allowed to go. Visual only.
		//int mBlipCapRight, mBlipCapLeft;
	};
}
#endif