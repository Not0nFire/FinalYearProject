#ifndef _MINION_FACTORY_H
#define _MINION_FACTORY_H

#include <include/TinyXML2/tinyxml2.h>
#include <include/Minion.hpp>
#include <include/Hero.hpp>

class MinionFactory {
public:
	MinionFactory();
	~MinionFactory();

	Minion* produce(std::string minionType, sf::Vector2f const &position, Path const &path);
	Hero* produce(std::string minionType, sf::Vector2f const &position);
private:
	//! Minion definitions mapped to by their file names
	std::map<std::string, std::unique_ptr<PawnDef>> mMinionDefMap;

	/*!
	\brief Creates a MinionDef from an xml file.
	Adds the created MinionDef to the map.
	\param minionType The name of the file.
					  The path used will be mPathToXMLFolder + minionType + mDefExtension.
	*/
	void createDefFromFile(std::string const &minionType);

	//! The path to the folder that contains the minion def xml files
	const char * mPathToXmlFolder;
	//! The extension used to minion def xml files. (e.g. '.xml' or '.def')
	const char * mDefExtension;
};
#endif
