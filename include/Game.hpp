#ifndef _GAME_H
#define _GAME_H

#include <include/ResourceManager.hpp>
#include <include/Renderer.hpp>
#include <include/Level.hpp>
#include <include/Menu.hpp>
#include <include/SceneManager.hpp>

using namespace boost;
using namespace signals2;

class Game {
private:
	Renderer mRenderer;

	bool mRun, mPaused;

	void handleEvent(sf::Event &event);

	Level* mLevel;
	Menu* mMenu;

public:
	Game();
	~Game();

	/*!
	\brief Begins the game.
	Intended usage:
		in main(): return game.run();

	Returns EXIT_SUCCESS (0)
	*/
	int run();
};
#endif