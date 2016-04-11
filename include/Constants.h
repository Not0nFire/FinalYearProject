#ifndef GAME_CONSTANTS_HPP
#define GAME_CONSTANTS_HPP

#include <string>
#include <SFML/System/Vector2.hpp>

namespace Constants
{
	class Strings {
	public:
		//! This class cannot be instanciated.
		Strings() = delete;

		static std::string const& getGameName();	//!< The name of the game.
		static std::string const& getPauseDialogueTitle();	//!< Title text for the in-game pause dialogue box.
		static std::string const& getPauseDialogueBody();	//!< Body text for the in-game pause dialogue box.
		static std::string const& getPauseDialogueYES();	//!< Confirmation text for in-game pause dialogue box.
		static std::string const& getPauseDialogueNO();		//!< Declination text for in-game pause dialogue box.
	};

	class Vectors {
	public:
		//! This class cannot be instanciated.
		Vectors() = delete;

		static sf::Vector2f const& getViewport();		//!< Viewport for the camera
		static sf::Vector2f const& getCameraBounds();	//!< Camera edges cannot go beyond this volume (starts at 0,0)
	};
};
#endif