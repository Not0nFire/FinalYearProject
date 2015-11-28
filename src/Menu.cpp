#include "../include/Menu.hpp"

Menu::Menu(sfg::Box::Orientation orientation, float spacing) {
	mBox = sfg::Box::Create(orientation, spacing);
	mBox->SetRequisition(sf::Vector2f(200, 400));
	mDesktop.Add(mBox);
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

void Menu::addLabel(sf::String const& label, sf::Vector2f const& minSize) {
	sfg::Label::Ptr lbl = sfg::Label::Create(label);
	lbl->SetRequisition(minSize);
	mBox->Pack(lbl);
}

bool Menu::handleEvent(sf::Event &Event) {
	mDesktop.HandleEvent(Event);
	return true;
}

void Menu::draw( sf::RenderWindow &w ) {
}

void Menu::update(sf::Time const &elapsedTime) {
	mDesktop.Update(elapsedTime.asSeconds());
}
