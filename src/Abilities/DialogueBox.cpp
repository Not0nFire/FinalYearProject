#include <include/Gui/DialogueBox.hpp>

using namespace gui;

DialogueBox::DialogueBox(sf::Vector2f const& centre, sf::Vector2f const& size, std::string const& title, std::string const& message, std::string const& yesText, std::string const& noText, unsigned int titleCharSize, unsigned int msgCharSize) :
mResult(CANCEL),
mTitle(title, ResourceManager<sf::Font>::instance()->get("./res/fonts/KENVECTOR_FUTURE.TTF"), titleCharSize),
mMessage(message, ResourceManager<sf::Font>::instance()->get("./res/fonts/KENVECTOR_FUTURE.TTF"), msgCharSize),
mIsOpen(false),
mBoxGraphic(size)
{
	const auto textColor = sf::Color(73, 40, 17);

	//Setup box
	mBoxGraphic.setOrigin(size.x * 0.5f, size.y * 0.5f);
	mBoxGraphic.setPosition(centre);
	mBoxGraphic.setTexture(&ResourceManager<sf::Texture>::instance()->get("./res/img/dialogue_box.png"));

	//Set message origin and colour
	auto txtBounds = mMessage.getGlobalBounds();
	mMessage.setOrigin(txtBounds.width * 0.5f, txtBounds.height * 0.5f);
	mMessage.setColor(textColor);

	//Set title origin, colour and make it bold
	txtBounds = mTitle.getGlobalBounds();
	mTitle.setOrigin(txtBounds.width * 0.5f, txtBounds.height * 0.5f);
	mTitle.setColor(textColor);
	mTitle.setStyle(sf::Text::Style::Bold);

	//Load basic button definiton xml
	tinyxml2::XMLDocument doc;
	auto result = doc.LoadFile("./res/xml/basic_buttons.xml");
	if (result != tinyxml2::XML_NO_ERROR) {
		throw result;
	}

	//Setup positions for text and buttons
	mTitle.setPosition(centre.x, centre.y - size.y * 0.3f);
	mMessage.setPosition(centre.x, centre.y);
	mButtonMap	[YES]		= std::make_unique<TextButton>(centre.x - size.x * 0.25f, centre.y + size.y * 0.3f, doc.FirstChildElement("BlankButtonSmall"), yesText);
	mButtonMap	[NO]		= std::make_unique<TextButton>(centre.x + size.x * 0.25f, centre.y + size.y * 0.3f, doc.FirstChildElement("BlankButtonSmall"), noText);
	mButtonMap	[CANCEL]	= std::make_unique<Button>(centre.x + size.x * 0.4f, centre.y - size.y * 0.4f, doc.FirstChildElement("CloseButton"));
}

DialogueBox::~DialogueBox() {}

void DialogueBox::open() {
	mIsOpen = true;
}

bool DialogueBox::isOpen() const {
	return mIsOpen;
}

bool DialogueBox::handleEvent(sf::Event const& theEvent) {
	bool handled = false;
	switch (theEvent.type) {
		
		case sf::Event::MouseButtonPressed:
			handled = true;
			updateButtons(sf::Vector2i(theEvent.mouseButton.x, theEvent.mouseButton.y));
			updateResult();
			break;

		case sf::Event::MouseMoved:
			handled = true;
			updateButtons(sf::Vector2i(theEvent.mouseMove.x, theEvent.mouseMove.y));
			break;

		default:
			//do nothing
			break;
	}

	return handled;
}

DialogueBox::Result DialogueBox::getResult() const {
	return mResult;
}

void DialogueBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	//draw the box
	target.draw(mBoxGraphic, states);

	//draw the title text
	target.draw(mTitle, states);

	//draw the message test
	target.draw(mMessage, states);

	//draw the buttons
	for (auto const& pair : mButtonMap) {
		target.draw(*pair.second, states);
	}
}

void DialogueBox::updateButtons(sf::Vector2i const& mousePosition) {
	for (auto& pair : mButtonMap) {
		pair.second->update(mousePosition);
	}
}

void DialogueBox::updateResult() {
	for (auto& pair : mButtonMap) {
		//if the button was clicked...
		if (mIsOpen && pair.second->checkClick()) {
			//...set the result to be its key in the map
			mResult = pair.first;
			mIsOpen = false;	//close the dialogue
		}
	}
}
