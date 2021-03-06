#ifndef UNIT_FACTORY_H
#define UNIT_FACOTRY_H

#include <map>
#include <list>
#include <include/Minion.hpp>
#include <include/Hero.hpp>
#include <include/TinyXML2/tinyxml2.h>

using namespace tinyxml2;

//! Factory for production of Hero and Minions from xml definition files.
class UnitFactory {
public:
	/*!
	\param definitionFolderPath Path to a folder containing unit definition files
	\param defFileExtension File extension of unit definition files
	*/
	UnitFactory(std::string const &definitionFolderPath="./res/xml/", std::string const& defFileExtension=".def");
	~UnitFactory();

	/*!
	\brief Creates a new Pawn derivative from the definition at path.
	\param name Name of file containing definition, without the file extension.
	\remarks Valid units are Hero and Minion; The root node of the file needs to match one of these.
	*/
	std::shared_ptr<Pawn> produce(std::string const &name);

private:
	//! Map of file paths to currently open xml documents.
	std::map<std::string, XMLDocument*> mDefinitionMap;

	/*!
	\brief Opens an xml file and holds it in mDefinitionMap, with its path as the key.
	*/
	void loadDefinition(std::string const &name);

	std::string mDefinitionFolderPath;	//!< Path to folder containing xml unit definitions.
	std::string mFileExt;	//!< File extension of xml unit definitions.
};
#endif