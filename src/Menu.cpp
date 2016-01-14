#include "../include/Menu.hpp"

Menu::Menu(std::shared_ptr<sfg::SFGUI> gui, std::string const &themePath, sfg::Box::Orientation orientation, float spacing) :
mSFGUI(gui)
{
	mDesktop.LoadThemeFromFile(themePath);
	mBox = sfg::Box::Create(orientation, spacing);
	mBox->SetRequisition(sf::Vector2f(200, 400));
	mBox->SetPosition(sf::Vector2f(200, 300));
	mDesktop.Add(mBox);

	mBgMusic.openFromFile("./res/mus/menu.ogg");
	mBgMusic.setLoop(true);
}

Menu* Menu::fromXML(const char* path) {
	throw "Not Implemented -- Requires TinyXML2";
	return nullptr;
}

void Menu::addButton(sf::String const& label, std::function<void()> onClick, sf::Vector2f const& minSize) {
	sfg::Button::Ptr btn = sfg::Button::Create(label);
	btn->SetRequisition(minSize);
	btn->GetSignal(sfg::Button::OnLeftClick).Connect(onClick);
	mBox->Pack(btn);
}

void Menu::addLabel(sf::String const& label, bool title, sf::Vector2f const& minSize) {
	sfg::Label::Ptr lbl = sfg::Label::Create(label);
	if (title) {
		lbl->SetId("title");
	}
	lbl->SetRequisition(minSize);
	mBox->Pack(lbl);
}

bool Menu::handleEvent(sf::Event &Event) {
	mDesktop.HandleEvent(Event);
	return true;
}

void Menu::cleanup() {
	mBox->Show(false);	//hide sfgui widgets
	mBgMusic.stop();
}


void Menu::draw( sf::RenderWindow &w ) {
	mSFGUI->Display(w);
}

void Menu::update(sf::Time const &elapsedTime) {
	mBox->Show();
	mDesktop.Update(elapsedTime.asSeconds());

	if (mBgMusic.getStatus() != sf::Music::Status::Playing) {
		mBgMusic.play();
	}
}
