#ifndef _GAME_H
#define _GAME_H

#include <include/ResourceManager.hpp>
#include <include/Renderer.hpp>
#include <include/Level.hpp>
#include <include/MainMenu.hpp>
#include <include/LevelSelect.hpp>
#include <include/SceneManager.hpp>
#include <include/TinyXML2/tinyxml2.h>
#include "PlayerProfile.hpp"

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

	static std::unique_ptr<PlayerProfile> mPlayerProfile;

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

	/*!
	\brief Gets the active player's profile.
	The calling class does not obtain ownership.
	\returns A reference to the player's profile. Ownership is not given.
	*/
	static PlayerProfile& getPlayerProfile();
};
#endif