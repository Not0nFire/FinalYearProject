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
	static const std::string path = "./res/fonts/NEVIS.TTF";
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
std::string const& Strings::getCompletionDialogueTitle() {
	static const std::string title = "Huzzah!";
	return title;
}

std::string const& Strings::getCompletionDialogueBody() {
	static const std::string body = "You have completed this level!";
	return body;
}

std::string const& Strings::getCompletionDialogueYES() {
	static const std::string yes = "Level Select";
	return yes;
}

std::string const& Strings::getCompletionDialogueNO() {
	static const std::string no = "Main Menu";
	return no;
}

std::string const& Strings::getFailDialogueTitle() {
	static const std::string title = "Oh no!";
	return title;
}

std::string const& Strings::getFailDialogueBody() {
	static const std::string body = "You failed this level.\nBetter luck next time.";
	return body;
}

std::string const& Strings::getFailDialogueYES() {
	static const std::string yes = "Level Select";
	return yes;
}

std::string const& Strings::getFailDialogueNO() {
	static const std::string no = "Main Menu";
	return no;
}

std::string const& Strings::getLivesTexture() {
	static const std::string path = "./res/img/heart.png";
	return path;
}

std::string const& Strings::getMoneyTexture() {
	static const std::string path = "./res/img/coin.png";
	return path;
}

std::string const& Strings::getPortraitTexture() {
	static const std::string path = "./res/img/portrait.png";
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
//	Number constants - Section start	//
//										//
//////////////////////////////////////////
#pragma region number_constants
float Numbers::getCameraMoveSpeed() {
	static auto speed = 250.f;
	return speed;
}

float Numbers::getCameraEdgeMoveThreshold() {
	static auto threshold = 20.f;
	return threshold;
}

unsigned Numbers::getMageTowerMaxTargets() {
	static auto max = 3u;
	return max;
}

unsigned Numbers::getHeroMoveSoundChance() {
	static auto chance = 33u;
	return chance;
}
#pragma endregion 
//////////////////////////////////////////
//										//
//	Number constants - Section end		//
//										//
//////////////////////////////////////////

//////////////////////////////////////////
//										//
//	Vector constants - Section start	//
//										//
//////////////////////////////////////////
#pragma region vector_constants
sf::Vector2f const& Vectors::getCameraBounds() {
	static const sf::Vector2f bounds(4000.f, 4000.f);
	return bounds;
}

sf::Vector2u const& Vectors::getMinimumResolution() {
	static const sf::Vector2u min(800u, 600u);
	return min;
}

sf::Vector2u const& Vectors::getScreenSize() {
	static const sf::Vector2u size(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
	return size;
}

sf::Vector2f const& Vectors::getLivesPosition() {
	static const sf::Vector2f position(1240.f, 10.f);
	return position;
}

sf::Vector2f const& Vectors::getLivesScale() {
	static const sf::Vector2f scale(1.f, 1.f);
	return scale;
}

sf::Vector2f const& Vectors::getLivesSpacing() {
	static const sf::Vector2f spacing(0.f, 30.f);
	return spacing;
}

sf::Vector2f const& Vectors::getMoneyIconPosition() {
	static const sf::Vector2f position(680.f, 680.f);
	return position;
}

sf::Vector2f const& Vectors::getMoneyPosition() {
	static const sf::Vector2f position(700.f, 680.f);
	return position;
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
#pragma region misc_constants
sf::Color const& Misc::getPrimaryTextColor() {
	static const sf::Color colour(73, 40, 17);
	return colour;
}

sf::Color const& Misc::getSecondaryTextColor() {
	static const sf::Color colour(255, 191, 0);
	return colour;
}
#pragma endregion
//////////////////////////////////////////
//										//
//	Misc constants - Section end		//
//										//
//////////////////////////////////////////