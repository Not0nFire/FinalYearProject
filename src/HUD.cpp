#include <include/HUD.hpp>

namespace HUD_Detail {
	HealthBarStatic::HealthBarStatic(Pawn* pwn, sf::Vector2f const &location, sf::Vector2f const &size, sfg::Desktop &desktop) :
		mPawnToTrack(pwn),
		mBar(sfg::ProgressBar::Create()),
		M_MAX_HEALTH(mPawnToTrack->getHealth())
	{
		mBar->SetPosition(location);
		mBar->SetRequisition(size);
		mBar->SetFraction(1.f);
		mBar->SetId("health");
		desktop.Add(mBar);
	}

	void HealthBarStatic::update() {
		mBar->SetFraction(mPawnToTrack->getHealth() / M_MAX_HEALTH);
	}

	HealthBarFollowing::HealthBarFollowing(Pawn* pwn, sf::Vector2f const& offset, sf::Vector2f const& size, sfg::Desktop& desktop) :
		HealthBarStatic(pwn, pwn->getPosition() + offset, size, desktop),
		mOffset(offset)
	{
	}

	void HealthBarFollowing::update() {
		HealthBarStatic::update();
		mBar->SetPosition(mPawnToTrack->getPosition() + mOffset);
	}
}//end namespace HUD_Detail

HUD::HUD(std::shared_ptr<sfg::SFGUI> sfgui) :
mSFGUI(sfgui),
mWidgets(),
mDesktop(),
mIsShown(true)
{
}

HUD::~HUD() {
	
}

void HUD::update(sf::Time const& elapsedTime) {
	for(auto healthBar : mHealthBars) {
		healthBar->update();
	}

	mDesktop.Update(elapsedTime.asSeconds());
}

void HUD::draw(sf::RenderWindow& window) const {
	mSFGUI->Display(window);
}

void HUD::addHealthBar(Pawn* pawn, sf::Vector2f const &offset, sf::Vector2f const &size) {
	mHealthBars.push_back(new HUD_Detail::HealthBarFollowing(pawn, offset, size, mDesktop));
}

void HUD::addHealthBarStatic(Pawn* pawn, sf::Vector2f const &location, sf::Vector2f const &size) {
	mHealthBars.push_back(new HUD_Detail::HealthBarStatic(pawn, location, size, mDesktop));
}

void HUD::show() {
	for (auto w : mWidgets) {
		w->Show(true);
	}

	mIsShown = true;
}

void HUD::hide() {
	for (auto w : mWidgets) {
		w->Show(false);
	}

	mIsShown - false;
}

bool HUD::isShown() const {
	return mIsShown;
}
