#if _DEBUG
#pragma comment(lib, "sfml-graphics-d.lib")
#pragma comment(lib, "sfml-main-d.lib")
#pragma comment(lib, "sfml-network-d.lib")
#pragma comment(lib, "sfml-system-d.lib")
#pragma comment(lib, "sfml-window-d.lib")
#pragma comment(lib, "sfml-audio-d.lib")
#pragma comment(lib, "sfgui-d.lib")
#pragma comment(lib, "thor-d.lib")
#else
#pragma comment(lib, "sfml-graphics.lib")
#pragma comment(lib, "sfml-main.lib")
#pragma comment(lib, "sfml-network.lib")
#pragma comment(lib, "sfml-system.lib")
#pragma comment(lib, "sfml-window.lib")
#pragma comment(lib, "sfml-audio.lib")
#pragma comment(lib, "sfgui.lib")
#pragma comment(lib, "thor.lib")
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")	//get rid of console window for release build
#endif


#include <include/Game.hpp>
#include <include/Constants.h>
#include <Windows.h>

void checkResolution() {
	//Check that screen supports a playable resolution.
	auto const& bestMode = sf::VideoMode::getFullscreenModes().at(0);
	auto const& minRes = Constants::Vectors::getMinimumResolution();

	if (bestMode.width < minRes.x || bestMode.height < minRes.y) {

		MessageBox(nullptr, "Screen resolution is too low.\nGame may be unplayable.", "Unsupported resolution!", MB_OK | MB_ICONWARNING);
	}
}

int main() {
	checkResolution();

	Game game;
	return game.run();
}
