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

#include <include/Settings.hpp>
#include <include/Constants.h>

int main() {

	std::cout << Constants::Strings::getGameName() << std::endl;

	printf("Resolution: %u, %u\n", Settings::getUnsigned("ResolutionX"), Settings::getUnsigned("ResolutionY"));
	printf("\nVolumes:\n\tMaster:\t\t%d\n\tMusic:\t\t%d\n\tEffects:\t%d\n", Settings::getInt("MasterVolume"), Settings::getInt("MusicVolume"), Settings::getInt("EffectsVolume"));

	system("PAUSE");

	Game game;
	return game.run();
}