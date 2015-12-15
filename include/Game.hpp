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

	std::shared_ptr<sfg::SFGUI> mSFGUI;

public:
	/*!
	\brief Sets up the game. Takes no arguements.
	*/
	Game();
	~Game();

	/*!
	\brief Begins the game.
	\remarks Intended usage: in main(): return game.run();
	\returns EXIT_SUCCESS (0)
	*/
	int run();
};
#endif