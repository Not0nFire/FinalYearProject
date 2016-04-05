#include <include/HUD.hpp>

namespace gui
{
	HudItem::HudItem() : mRemovalFlag(false) {
		//empty ctor body
	}

	void HudItem::update(float deltaSeconds) {
		//do nothing
	}

	bool HudItem::isFlaggedForRemoval() const {
		return mRemovalFlag;
	}

	void HudItem::flagForRemoval() {
		mRemovalFlag = true;
	}

	//////////////////////////////
	//	HealthBarStatic - start	//
	//////////////////////////////
	HealthBarStatic::HealthBarStatic(std::shared_ptr<Pawn> const& pawnToTrack, sf::Texture* texture, sf::Vector2f const& position, sf::Vector2f const& size) :
		mBar(size),
		mTracked(pawnToTrack),
		M_MAX_HEALTH(pawnToTrack->getHealth())
	{
		if (nullptr != texture) {
			mBar.setTexture(texture);
		}
		else {
			mBar.setFillColor(sf::Color::Red);
		}
		mBar.setPosition(position);

	}

	void HealthBarStatic::update(float deltaSeconds) {
		if (auto pawn = mTracked.lock()) {
			mBar.setScale(pawn->getHealth() / M_MAX_HEALTH, 1.f);
		}
		else {
			flagForRemoval();	//if pawn does not exist anymore
		}
	}

	void HealthBarStatic::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(mBar, states);
	}
	//////////////////////////////
	//	HealthBarStatic - end	//
	//////////////////////////////

	//////////////////////////
	//	HealthBar - start	//
	//////////////////////////
	HealthBar::HealthBar(std::shared_ptr<Pawn> const& pawnToTrack, sf::Texture* texture, sf::Vector2f const& offset, sf::Vector2f const& size) :
		HealthBarStatic(pawnToTrack, texture, pawnToTrack->getPosition(), size),
		M_OFFSET(offset)
	{}

	void HealthBar::update(float deltaSeconds) {
		if (auto pawn = mTracked.lock()) {
			mBar.setPosition(pawn->getPosition() + M_OFFSET);
		}

		HealthBarStatic::update(deltaSeconds);
	}
	//////////////////////////
	//	HealthBar - end		//
	//////////////////////////

	//////////////////////////
	//	LifeTracker - start	//
	//////////////////////////
	LifeTracker::LifeTracker(std::shared_ptr<int> lives, sf::Texture& texture, sf::Vector2f const& position, sf::Vector2f const& scale, sf::Vector2f const& spacing) :
		mSprite(texture),
		M_SPACING(spacing),
		mLives(lives)
	{
		mSprite.setScale(scale);
		mSprite.setPosition(position);
	}

	void LifeTracker::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		const auto lives = *mLives;
		sf::Sprite temp = mSprite;
		for (auto i = 0; i < lives; ++i) {
			target.draw(temp, states);
			temp.move(M_SPACING);
		}
	}
	//////////////////////////
	//	LifeTracker - end	//
	//////////////////////////

	//////////////////////////
	//	Image -	start		//
	//////////////////////////
	Image::Image(sf::Texture& texture, sf::Vector2f const& position, sf::Vector2f const& scale) :
		mSprite(texture)
	{
		mSprite.setPosition(position);
		mSprite.setScale(scale);
	}

	void Image::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(mSprite, states);
	}
	//////////////////////////
	//	Image - end			//
	//////////////////////////
}

Hud::Hud() {}

Hud::~Hud() {}

void Hud::addHealthBar(std::shared_ptr<Pawn> const& pawnToTrack, sf::Vector2f const& offsetOrPosition, sf::Vector2f const& size, sf::Texture* texture, bool stationary) {
	std::unique_ptr<gui::HudItem> item;

	if (stationary) {
		//Create a stationary healthbar
		item = std::make_unique<gui::HealthBarStatic>(pawnToTrack, texture, offsetOrPosition, size);
	}
	else {
		//Create a healthbar that follows the pawn it tracks
		item = std::make_unique<gui::HealthBar>(pawnToTrack, texture, offsetOrPosition, size);
	}

	mItems.push_back(move(item));
}

void Hud::addLifeTracker(std::shared_ptr<int> const& livesToTrack, sf::Texture& texture, sf::Vector2f const& position, sf::Vector2f const& scale, sf::Vector2f const& spacing) {
	std::unique_ptr<gui::HudItem> item;

	item = std::make_unique<gui::LifeTracker>(livesToTrack, texture, position, scale, spacing);

	mItems.push_back(move(item));
}

void Hud::addImage(sf::Texture& texture, sf::Vector2f const& position, sf::Vector2f const& scale) {
	std::unique_ptr<gui::HudItem> item;

	item = std::make_unique<gui::Image>(texture, position, scale);

	mItems.push_back(move(item));
}

void Hud::update(float deltaSeconds) {
	
	auto itr = mItems.begin();
	while ( itr != mItems.end()) {
		auto item = itr->get();

		item->update(deltaSeconds);

		if (item->isFlaggedForRemoval()) {
			itr = mItems.erase(itr);
		} else {
			++itr;
		}
	}
}

void Hud::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (auto const& itemPtr : mItems) {
		target.draw(*itemPtr, states);
	}
}
