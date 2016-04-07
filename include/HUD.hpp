#ifndef HUD_HPP
#define HUD_HPP

#include <SFML/Graphics.hpp>
#include "Pawn.hpp"

namespace gui
{
	/*!
	\class HudItem
	\brief Interface for non-interactable hud elements.
	Non-interactable hud elements are for show, the user can't click on or use them.
	Therefore, gui::Button does not derive from this class.
	\remarks The flagForRemoval() method allows derived classes to ask the containing hud to erase them.
	*/
	class HudItem : public sf::Drawable {
	public:
		HudItem();
		virtual void update(float deltaSeconds);
		bool isFlaggedForRemoval() const;
	protected:
		void flagForRemoval();
	private:
		bool mRemovalFlag;
	};

	//! Statically positioned bar that tracks a Pawn's health.
	class HealthBarStatic : public HudItem {
	public:
		HealthBarStatic(std::shared_ptr<Pawn> const& pawnToTrack, sf::Texture* texture, sf::Vector2f const& position, sf::Vector2f const& size);
		virtual void update(float deltaSeconds) override;
	protected:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		sf::RectangleShape mBar;	//!< Rectangle to represent health.
		std::weak_ptr<Pawn> mTracked;	//!< The pawn whose health we're tracking.
		const float M_MAX_HEALTH;	//!< The amount of health the pawn has when the healthbar was constructed. mBar is scales according to this.
	};

	//! Healthbar that follows a Pawn's position.
	class HealthBar : public HealthBarStatic {
	public:
		HealthBar(std::shared_ptr<Pawn> const& pawnToTrack, sf::Texture* texture, sf::Vector2f const& offset, sf::Vector2f const& size);
		void update(float deltaSeconds) override;
	private:
		const sf::Vector2f M_OFFSET;	//!< Displacement from pawn at which to draw the healthbar.
	};

	//! Draws a line of images, one for each life (or whatever other int it may be told to track).
	class LifeTracker : public HudItem {
	public:
		LifeTracker(std::shared_ptr<int> lives, sf::Texture& texture, sf::Vector2f const& position, sf::Vector2f const& scale = { 1.f, 1.f }, sf::Vector2f const& spacing = {0.f, 0.f});
	protected:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	private:
		sf::Sprite mSprite;
		const sf::Vector2f M_SPACING;	//!< Space between lives on screen. Does not account for width of mSprite.
		std::shared_ptr<int> mLives;	//!< Shared pointer to an int. We'll draw this many lives on the screen.
	};

	//! Draws a sprite.
	class Image : public HudItem {
	public:
		Image(sf::Texture& texture, sf::Vector2f const& position, sf::Vector2f const& scale = { 1.f, 1.f });
	protected:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	private:
		sf::Sprite mSprite;
	};

	//! Outputs a number from a shared pointer as text
	class NumberTracker : public HudItem {
	public:
		NumberTracker(std::shared_ptr<int> const& numberToTrack, sf::Vector2f const& position, sf::Font const& font, unsigned int characterSize = 30u, sf::Vector2f const& scale = { 1.f, 1.f });
		void update(float deltaSeconds) override;
	protected:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	private:
		std::shared_ptr<int> const& mNumber;
		sf::Text mText;
	};
}//end namespace hud_detail

class Hud : public sf::Drawable {
public:
	Hud();
	~Hud();

	/*!
	\brief Adds a healthbar to the hud.
	\param pawnToTrack The pawn whose health will be displayed by the bar.
	\param offsetOrPosition The offset (for following healthbars) or position (for stationary healthbars) at which to draw the bar. Offset is an offset from the pawn.
	\param size The size of the bar at full health.
	\param texture Texture to draw the bar with. If NULL, will default to solid red.
	\param stationary True if the bar is to be non-moving. (Placed statically on the screen).
	\remarks The pawn's health at time of construction is what the healthbar will consider its max health.
	*/
	void addHealthBar(std::shared_ptr<Pawn> const& pawnToTrack, sf::Vector2f const& offsetOrPosition, sf::Vector2f const& size, sf::Texture* texture = nullptr, bool stationary = false);

	/*!
	\brief Adds a series of images to the hud. The number of images reflects the value of a shared int.
	\param livesToTrack Shared pointer to the number of images to draw. (e.g. Lives, Deaths, Potions...)
	\param position The upper left corner of the first image. This is the anchor.
	\param scale How much to scale the image by. All images will be drawn the same size, this is an overall scale.
	\param spacing The space between each image. This is measured from the top left of the last image drawn.
	*/
	void addLifeTracker(std::shared_ptr<int> const& livesToTrack, sf::Texture &texture, sf::Vector2f const& position, sf::Vector2f const& scale = { 1.f, 1.f }, sf::Vector2f const& spacing = { 0.f, 0.f });

	/*!
	\brief Adds an image to the hud.
	\param texture Texture to be drawn.
	\param position Upper left corner of image to be drawn.
	\param scale Absolute scale of the image, where {1, 1} is original texture size.
	*/
	void addImage(sf::Texture &texture, sf::Vector2f const& position, sf::Vector2f const& scale = { 1.f, 1.f });

	/*!
	\brief Adds a number to the hud.
	\param number Shared number to be displayed
	\param position Location of number of screen (upper left corner).
	\param font The font to use for the text.
	\param charSize The character size of the text.
	\param scale How much the scale the text by.
	*/
	void addNumberTracker(std::shared_ptr<int> const& number, sf::Vector2f const& position, sf::Font const& font, unsigned int charSize = 30u, sf::Vector2f const& scale = { 1.f, 1.f });

	//! Updates all hud items.
	void update(float deltaSeconds);

protected:
	//! Draws all hud items.
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	//! Container of hud item pointers.
	std::list<std::unique_ptr<gui::HudItem>> mItems;
};

#endif