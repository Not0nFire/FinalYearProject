#include <include/Constants.h>;
#include <SFML/Window/VideoMode.hpp>

using namespace Constants;

//////////////////////////////////////////
//										//
//	String constants - Section start	//
//										//
//////////////////////////////////////////
#pragma region string_constants
std::string const& Strings::getGameName() {
	static const std::string gameName = "FinalYearProject";
	return gameName;
}

std::string const& Strings::getMainFontPath() {
	static const std::string path = "./res/fonts/KENVECTOR_FUTURE.TTF";
	return path;
}

std::string const& Strings::getPauseDialogueTitle() {
	static const std::string title = "PAUSE";
	return title;
}
std::string const& Strings::getPauseDialogueBody() {
	static const std::string body = "The game is paused.\n\n\t\tDon't worry,\n\t\tI'll wait.";
	return body;
}
std::string const& Strings::getPauseDialogueYES() {
	static const std::string yes = "Resume";
	return yes;
}
std::string const& Strings::getPauseDialogueNO() {
	static const std::string no = "Quit";
	return no;
}
std::string const& Strings::getSettingsFilePath() {
	static const std::string path = "./res/xml/settings.xml";
	return path;
}
std::string const& Strings::getSettingsDialogueTitle() {
	static const std::string title = "Save?";
	return title;
}
std::string const& Strings::getSettingsDialogueBody() {
	static const std::string body = "Save these settings or\njust use them this once?";
	return body;
}
std::string const& Strings::getSettingsDialogueYES() {
	static const std::string yes = "Save";
	return yes;
}
std::string const& Strings::getSettingsDialogueNO() {
	static const std::string no = "Use";
	return no;
}
#pragma endregion
//////////////////////////////////////////
//										//
//	String constants - Section end		//
//										//
//////////////////////////////////////////

//////////////////////////////////////////
//										//
//	Vector constants - Section start	//
//										//
//////////////////////////////////////////
sf::Vector2f const& Vectors::getCameraBounds() {
	static const sf::Vector2f bounds(2000.f, 2000.f);
	return bounds;
}

sf::Vector2u const& Vectors::getMinimumResolution() {
	static const sf::Vector2u min(800u, 600u);
	return min;
}
#pragma endregion
//////////////////////////////////////////
//										//
//	Vector constants - Section end		//
//										//
//////////////////////////////////////////

//////////////////////////////////////////
//										//
//	Misc constants - Section start		//
//										//
//////////////////////////////////////////
sf::Color const& Misc::getPrimaryTextColor() {
	static const sf::Color colour(73, 40, 17);
	return colour;
}

sf::Color const& Misc::getSecondaryTextColor() {
	static const sf::Color colour(255, 191, 0);
	return colour;
}

//////////////////////////////////////////
//										//
//	Misc constants - Section end		//
//										//
//////////////////////////////////////////