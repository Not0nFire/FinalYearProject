#ifndef PLAYER_PROFILE_HPP
#define PLAYER_PROFILE_HPP

#include <string>
#include <map>
#include "include/TinyXML2/tinyxml2.h"
#include <set>

/*!
\class PlayerProfile
\brief Record of how far a player has progressed through the game.
Used to save/load to/from file.
*/
class PlayerProfile {
public:
	/*!
	\brief Constructs a PlayerProfile from an xml file. If the file does not exist, a new one will be created.
	*/
	PlayerProfile(std::string const &xmlPath);
	~PlayerProfile();

	/*!
	\brief Find out if the player has unlocked a level.
	\param levelPath Path to level xml definition. (Not opened, just used as a key)
	\returns True if the player has unlocked the level, false otherwise.
	*/
	bool hasUnlockedLevel(std::string const &levelPath);

	/*!
	\brief Unlocks a level for this profile.
	\param levelPath Path to level xml definition. (Not opened, just used as a key)
	*/
	void unlockLevel(std::string const &levelPath);

	//! Returns the name of the player.
	std::string const& getName() const;

	/*!
	\brief Saves the profile to its file.
	\returns True if saving was successful.
	*/
	bool save() const;

private:
	//! Every level path contained in the set is considered unlocked for this profile.
	std::set<std::string> mUnlockedLevelPaths;

	//! Name of the player.
	std::string mName;

	const std::string mPath;

	/*!
	\brief Creates a new profile file with the first level unlocked.
	zparam path Path to where the file should be created, including file name and extension.
	*/
	static void createProfileFile(std::string const& path);

	static void printError(std::string const &preMsg, errno_t error);
};
#endif