#include "include/PlayerProfile.hpp"

PlayerProfile::PlayerProfile(std::string const& xmlPath) : mPath(xmlPath) {
	using namespace tinyxml2;
	XMLDocument doc;
	XMLError result = doc.LoadFile(xmlPath.c_str());

	if (result != XML_NO_ERROR) {
		if (result == XML_ERROR_FILE_NOT_FOUND) {
			//Create a new profile file if it doesn't exist.
			createProfileFile(xmlPath);

			result = doc.LoadFile(xmlPath.c_str());
			_ASSERT(result == XML_NO_ERROR);

		} else {
			throw result;
		}
	}

	auto root = doc.FirstChildElement("Profile");

	mName = root->Attribute("name");

	//For each <UnlockedLevel> element...
	for (auto unlockedLevel = root->FirstChildElement("Progress")->FirstChildElement("UnlockedLevel");
		unlockedLevel != nullptr;
		unlockedLevel = unlockedLevel->NextSiblingElement("UnlockedLevel"))
	{
		//...put its path into the set.
		mUnlockedLevelPaths.insert(unlockedLevel->Attribute("path"));
	}
}

PlayerProfile::~PlayerProfile() {}

bool PlayerProfile::hasUnlockedLevel(std::string const& levelPath) {
	return mUnlockedLevelPaths.find(levelPath) != mUnlockedLevelPaths.end();
}

void PlayerProfile::unlockLevel(std::string const& levelPath) {
	mUnlockedLevelPaths.insert(levelPath);
}

std::string const& PlayerProfile::getName() const {
	return mName;
}

bool PlayerProfile::save() const {
	using namespace tinyxml2;

	bool saveSuccessful = false;

	//Open the file in write mode.
	FILE* profile;
	errno_t error;

	//Write to a temporary file and replace original at the end. (Don't screw up the old saved profile if saving fails!)
	if ((error = fopen_s(&profile, (mPath+".tmp").c_str(), "wt")) != 0) {
		//Something went wrong when opening file.
		printError("Error opening file:", error);
	}
	else {
		//File opened successfully.

		XMLPrinter printer = XMLPrinter(profile);
		//Create the root <Profile> tag
		printer.OpenElement("Profile");

		//Push a name attribute onto the <Profile> tag
		printer.PushAttribute("name", mName.c_str());

		//Create the <Progress> tag
		printer.OpenElement("Progress");

		//Create <UnlockedLevel> tags for every level in the set.
		for (auto itr = mUnlockedLevelPaths.begin(); itr != mUnlockedLevelPaths.end(); ++itr) {
			//Create an <UnlockedLevel> tag and set its path attribute.
			printer.OpenElement("UnlockedLevel");
			printer.PushAttribute("path", itr->c_str());
			printer.CloseElement();	//</UnlockedLevel>
		}

		//Close all the open tags.
		printer.CloseElement();	//</Progress>
		printer.CloseElement();	//</Profile>

		//Close the file
		fclose(profile);

		//Try to remove the old profile file...
		error = remove(mPath.c_str());
		if (error != 0) {
			//...print the error if it fails.
			printError("Error removing old file:",error);
		}
		else {
			//...rename the ".tmp" from the new file.
			error = rename((mPath + ".tmp").c_str(), mPath.c_str());
			if (error != 0) {
				printError("Error overwriting profile:", error);
			} else {
				//If all of that worked, the save is a success!
				saveSuccessful = true;
			}
		}
	}

	return saveSuccessful;
}

void PlayerProfile::createProfileFile(std::string const& path) {
	using namespace tinyxml2;

	//Open (or in this case create) the file in write mode
	FILE* profile;
	errno_t error;
	if ((error = fopen_s(&profile, path.c_str(), "wt")) != 0) {
		//Something went wrong when opening file.
		printError("Error opening file:", error);
	}
	else {
		//File opened successfully.

		XMLPrinter printer = XMLPrinter(profile);
		//Create the root <Profile> tag
		printer.OpenElement("Profile");

		//Push a name attribute onto the <Profile> tag
		printer.PushAttribute("name", "no_name");

		//Create the <Progress> tag
		printer.OpenElement("Progress");

		//Create an <UnlockedLevel> tag and set its path attribute to be the first level. (This should always be unlocked for the player)
		printer.OpenElement("UnlockedLevel");
		printer.PushAttribute("path", "./res/xml/levelOne.lvl");

		//Close all the open tags.
		printer.CloseElement();	//</UnlockedLevel>
		printer.CloseElement();	//</Progress>
		printer.CloseElement();	//</Profile>

		//Close the file
		fclose(profile);
	}
}

void PlayerProfile::printError(std::string const &preMsg, errno_t error) {
	auto buffer = new char[100];
	strerror_s(buffer, 100, error);
	printf("%s %s", preMsg.c_str(),buffer);
	delete[] buffer;
}
