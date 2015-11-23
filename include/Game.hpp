#ifndef _GAME_H
#define _GAME_H

#include <include/ResourceManager.hpp>
#include <include/Renderer.hpp>
#include <include/Level.hpp>
#include <include/Menu.hpp>
#include <include/SceneManager.hpp>

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Desktop.hpp>
#include <SFGUI/Button.hpp>

using namespace boost;
using namespace signals2;

class Game {
private:
	Renderer mRenderer;
	signal<void(sf::Vector2f)> onMouseClick;

	sfg::Desktop mDesktop;	//Top level container for all sfgui widgets
	sfg::Button::Ptr mButton;	//testing sfgui

	bool mRun, mPaused;

	void handleEvent(sf::Event &event);

	Level* mLevel;
	Menu* mMenu;

public:
	Game();
	~Game();

	int run();
};
#endif