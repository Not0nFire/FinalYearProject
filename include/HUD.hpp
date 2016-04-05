#ifndef HUD_HPP
#define HUD_HPP

#include <SFML/Graphics.hpp>
#include "Pawn.hpp"

namespace hud_detail
{
	//! Interface for all HUD items.
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
		sf::RectangleShape mBar;
		std::weak_ptr<Pawn> mTracked;
		const float M_MAX_HEALTH;
	};

	//! Healthbar that follows a Pawn's position.
	class HealthBar : public HealthBarStatic {
	public:
		HealthBar(std::shared_ptr<Pawn> const& pawnToTrack, sf::Texture* texture, sf::Vector2f const& offset, sf::Vector2f const& size);
		void update(float deltaSeconds) override;
	private:
		const sf::Vector2f M_OFFSET;
	};

	//! Draws a line of images, one for each life (or whatever other int it may be told to track).
	class LifeTracker : public HudItem {
	public:
		LifeTracker(std::shared_ptr<int> lives, sf::Texture& texture, sf::Vector2f const& position, sf::Vector2f const& scale = { 1.f, 1.f }, sf::Vector2f const& spacing = {0.f, 0.f});
	protected:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	private:
		sf::Sprite mSprite;
		const sf::Vector2f M_SPACING;
		std::shared_ptr<int> mLives;
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
}//end namespace hud_detail

class Hud : public sf::Drawable {
public:
	Hud();
	~Hud();

	void addHealthBar(std::shared_ptr<Pawn> const& pawnToTrack, sf::Vector2f const& offsetOrPosition, sf::Vector2f const& size, sf::Texture* texture = nullptr, bool stationary = false);

	void addLifeTracker(std::shared_ptr<int> const& livesToTrack, sf::Texture &texture, sf::Vector2f const& position, sf::Vector2f const& scale = { 1.f, 1.f }, sf::Vector2f const& spacing = { 0.f, 0.f });

	void addImage(sf::Texture &texture, sf::Vector2f const& position, sf::Vector2f const& scale = { 1.f, 1.f });

	//! Updates all hud items.
	void update(float deltaSeconds);

protected:
	//! Draws all hud items.
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	//! Container of hud item pointers.
	std::list<std::unique_ptr<hud_detail::HudItem>> mItems;
};

#endif