#include <include/HUD.hpp>

HUD::HUD(std::shared_ptr<sfg::SFGUI> sfgui) :
mSFGUI(sfgui),
mWidgets(),
mDesktop()
{
}

HUD::~HUD() {
	
}

void HUD::update(sf::Time const& elapsedTime) {
	mDesktop.Update(elapsedTime.asSeconds());
}
