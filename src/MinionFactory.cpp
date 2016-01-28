#include <include/MinionCreation/MinionFactory.hpp>

MinionFactory::MinionFactory() :
mPathToXmlFolder("././res/xml/"),
mDefExtension(".def")
{
}

MinionFactory::~MinionFactory() {
}

Minion* MinionFactory::produce(std::string minionType, sf::Vector2f const &position, Path const &path) {
	PawnDef* def;
	try
	{
		def = mMinionDefMap.at(minionType).get();
	}
	catch (std::out_of_range &oor)
	{
		//the key didn't exist in the map, so we'll try to load it from file
		createDefFromFile(minionType);
		def = mMinionDefMap.at(minionType).get();
	}

	return new Minion(*def, position, path);
}

Hero* MinionFactory::produce(std::string minionType, sf::Vector2f const& position) {
	PawnDef* def;
	try
	{
		def = mMinionDefMap.at(minionType).get();
	}
	catch (std::out_of_range &oor)
	{
		//the key didn't exist in the map, so we'll try to load it from file
		createDefFromFile(minionType);
		def = mMinionDefMap.at(minionType).get();
	}

	return new Hero(*def, position);
}

#define GET_ELEMENT(str) root->FirstChildElement(str)->GetText()
void MinionFactory::createDefFromFile(std::string const &minionType) {
	using namespace tinyxml2;

	XMLDocument doc;

	XMLError result = doc.LoadFile((mPathToXmlFolder + minionType + mDefExtension).c_str());	//try to load the xml from file
	if ( result != XML_NO_ERROR )
		throw result;	//throw an error if one occured

	XMLElement* root = doc.FirstChildElement("MINION_DEF");

	//see #define GET_ELEMENT above this method
	PawnDef* def = new PawnDef();
	def->health = atoi(GET_ELEMENT("Health"));
	def->attackRange = atoi(GET_ELEMENT("AttackRange"));
	def->attackDamage = atoi(GET_ELEMENT("AttackDamage"));
	def->attacksPerSecond = atof(GET_ELEMENT("AttacksPerSecond"));
	def->movementSpeed = atoi(GET_ELEMENT("MovementSpeed"));

	def->texturePath = GET_ELEMENT("Texture");
	def->animationSet = GET_ELEMENT("AnimSet");

	def->armour = atof(GET_ELEMENT("Armour"));
	def->magicResist = atof(GET_ELEMENT("MagicResist"));

	std::string factionStr = GET_ELEMENT("Faction");
	std::string dmgTypStr = GET_ELEMENT("DamageType");

	if (factionStr == "PLAYER")
	{
		def->faction = Pawn::Faction::PLAYER;
	}//faction defaults to ENEMY, so no need to do check for that

	//find out which type of damage to use
	if (dmgTypStr == "PHYSICAL")
	{
		def->damageType = Damage::Type::PHYSICAL;
	}
	else if (dmgTypStr == "MAGICAL")
	{
		def->damageType = Damage::Type::MAGICAL;
	}
	else
	{
		throw "Invalid damage type.";
	}

	mMinionDefMap[minionType] = std::unique_ptr<PawnDef>(def);
}
