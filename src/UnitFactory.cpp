#include <include/UnitFactory.hpp>

UnitFactory::UnitFactory(std::string const &definitionFolderPath, std::string const& defFileExtension) :
mDefinitionFolderPath(definitionFolderPath),
mFileExt(defFileExtension)
{
}

UnitFactory::~UnitFactory() {
	//Delete each XMLDocument
	for (auto entry : mDefinitionMap) {
		delete entry.second;
	}
}

Pawn* UnitFactory::produce(std::string const& name) {
	//If definition hasn't been loaded yet
	if (!mDefinitionMap.count(name)) {
		loadDefinition(name);
	}

	XMLElement* root = mDefinitionMap.at(name)->FirstChildElement();
	std::string rootName = root->Name();

	Pawn* unit = nullptr;

	//Create unit as specified in xml
	if (rootName == "Minion") {
		unit = new Minion(root);
	} else if (rootName == "Hero") {
		unit = new Hero(root);
	} else {
		throw "Invalid root tag";
	}

	return unit;
}

void UnitFactory::loadDefinition(std::string const& name) {
	XMLDocument* doc = new XMLDocument();

	//Try to load the xml at the path specified
	XMLError result = doc->LoadFile((mDefinitionFolderPath + name + mFileExt).c_str());
	if (result != XML_NO_ERROR) {
		delete doc;
		throw result;
	}

	//Push the root node onto the map with the name as the key
	mDefinitionMap[name] = doc;
}
