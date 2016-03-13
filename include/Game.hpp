#ifndef _GAME_H
#define _GAME_H

#include <include/ResourceManager.hpp>
#include <include/Renderer.hpp>
#include <include/Level.hpp>
#include <include/Menu.hpp>
#include <include/SceneManager.hpp>
#include <include/TinyXML2/tinyxml2.h>

using namespace boost;
using namespace signals2;

/*
\brief The Game itself. The only thing that need be included in _main.cpp_.
Usage:
\code	Game game;
		return game.run();
*/
class Game {
private:
	//! Threaded Renderer with fixed timestep.
	Renderer mRenderer;

	static bool mRun, mPaused;

	void handleEvent(sf::Event &event);

	Level *mLevelOne, *mLevelTwo;
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

	static void close();
};
#endif