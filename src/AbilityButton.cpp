#include <include/Gui/AbilityButton.hpp>

using namespace gui;

AbilityButton::AbilityButton(int x, int y, const tinyxml2::XMLElement* xmlButtonDefinition, std::shared_ptr<Ability const> const& ability) :
Button(x, y, xmlButtonDefinition),
mAbility(ability),
mCooldownVisual(getSize())
{
	mCooldownVisual.setTexture(&ResourceManager<sf::Texture>::instance()->get("./res/img/ability_overlays.png"));

	mCooldownVisual.setScale(getScale());

	auto bounds = mCooldownVisual.getLocalBounds();
	mCooldownVisual.setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);

	mCooldownVisual.setPosition(getPosition());

	mCooldownAnimator.addAnimation(
		"Cooldown",	//name of the animation
		ResourceManager<thor::FrameAnimation>::instance()->get("./res/xml/cooldown_timer.anim"),	//path to animation definition
		sf::seconds(ability->getTotalCooldown())	//animation duration
		);

	mCooldownAnimator.addAnimation(
		"Active",
		ResourceManager<thor::FrameAnimation>::instance()->get("./res/xml/active_ability.anim"),
		ability->getCastDuration()
		);
}

AbilityButton::~AbilityButton() {}

void AbilityButton::updateCooldownVisuals(sf::Time const& elapsedTime) {
	//update cooldown graphic if it's playing...
	if (mCooldownAnimator.isPlayingAnimation()) {
		mCooldownAnimator.update(elapsedTime);	//update the animation
		mCooldownAnimator.animate(mCooldownVisual);	//apply the animation to the object
	}
	//...otherwise, start the animation if the ability is on cooldown
	else if (auto ability = mAbility.lock()) {
		if (ability->getRemainingCooldown() > 0.f) {
			mCooldownAnimator.playAnimation("Cooldown");
			mCooldownAnimator.update(sf::seconds(0.f));	//prevent entire spritesheet being drawn before first real animate() call
			disable();	//disable button
		}
		else if (ability->isActive()) {
			mCooldownAnimator.playAnimation("Active");
			mCooldownAnimator.update(sf::seconds(0.f));
			disable();
		}
		else {
			enable();
		}
	}
}

void AbilityButton::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	Button::draw(target, states);	//run base draw first
	if (mCooldownAnimator.isPlayingAnimation()) {
		target.draw(mCooldownVisual, states);
	}
}
