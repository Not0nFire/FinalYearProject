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

	void HealthBarStatic::hide() {
		mBar->Show(false);
	}

	void HealthBarStatic::show() {
		mBar->Show(true);
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

	LabelImagePair::LabelImagePair(std::shared_ptr<int> valueToTrack, sf::Texture const & tex, sf::Font const &fnt, sf::Vector2f const& position, sf::Vector2f offset, bool imageFirst) :
		mValueToTrack(valueToTrack)
	{
		mImage.setTexture(tex);
		mText.setFont(fnt);
		if (imageFirst)
		{
			mImage.setPosition(position);
			mText.setPosition(position + offset);
		}
		else
		{
			mText.setPosition(position);
			mImage.setPosition(position + offset);
		}
	}

	void LabelImagePair::update() {
		mText.setString(std::to_string(*mValueToTrack));
	}

	void LabelImagePair::draw(sf::RenderWindow & w) {
		w.draw(mImage);
		w.draw(mText);
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

	for (auto lblimg : mLabelImagePairs)
	{
		lblimg->update();
	}

	mDesktop.Update(elapsedTime.asSeconds());
}

void HUD::draw(sf::RenderWindow& window) const {
	window.setView(window.getDefaultView());

	for (auto lblimg : mLabelImagePairs)
	{
		lblimg->draw(window);
	}

	for (auto img : mImages)
	{
		window.draw(*img);
	}

	mSFGUI->Display(window);
}

void HUD::addHealthBar(Pawn* pawn, sf::Vector2f const &offset, sf::Vector2f const &size) {
	mHealthBars.push_back(new HUD_Detail::HealthBarFollowing(pawn, offset, size, mDesktop));
}

void HUD::addHealthBarStatic(Pawn* pawn, sf::Vector2f const &location, sf::Vector2f const &size) {
	mHealthBars.push_back(new HUD_Detail::HealthBarStatic(pawn, location, size, mDesktop));
}

void HUD::addImage(sf::Texture const& tex, sf::Vector2f position) {
	sf::Sprite* spr = new sf::Sprite(tex);
	spr->setPosition(position);
	mImages.push_back(spr);
}

void HUD::show() {
	for (auto w : mWidgets) {
		w->Show(true);
	}

	for (auto hb : mHealthBars) {
		hb->show();
	}

	mIsShown = true;
}

void HUD::hide() {
	for (auto w : mWidgets) {
		w->Show(false);
	}

	for (auto hb : mHealthBars) {
		hb->hide();
	}

	mIsShown = false;
}

bool HUD::isShown() const {
	return mIsShown;
}
