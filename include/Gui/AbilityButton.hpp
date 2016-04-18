#ifndef GUI_ABILITY_BUTTON_HPP
#define GUI_ABILITY_BUTTON_HPP

#include <memory>
#include <include/Abilities/Ability.hpp>
#include "Button.hpp"

namespace gui
{
	/*!
	\class AbilityButton
	\brief Button with an Ability and cooldown visualization
	*/
	class AbilityButton : public Button {
	public:
		AbilityButton(int x, int y, const tinyxml2::XMLElement* xmlButtonDefinition, std::shared_ptr<Ability const> const& ability);
		virtual ~AbilityButton();

		void updateCooldownVisuals(sf::Time const& elapsedTime);

	protected:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	private:
		sf::RectangleShape mCooldownVisual;
		thor::Animator<sf::RectangleShape, std::string> mCooldownAnimator;
		std::weak_ptr<Ability const> mAbility;
		sf::Text mHotkeyText;
	};
}
#endif