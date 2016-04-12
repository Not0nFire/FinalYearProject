#include <include/Constants.h>;

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
#pragma region vector_constants
sf::Vector2f const& Vectors::getViewport() {
	static const sf::Vector2f viewport(1280.f, 800.f);
	return viewport;
}

sf::Vector2f const& Vectors::getCameraBounds() {
	static const sf::Vector2f bounds(2000.f, 2000.f);
	return bounds;
}
#pragma endregion
//////////////////////////////////////////
//										//
//	Vector constants - Section end		//
//										//
//////////////////////////////////////////