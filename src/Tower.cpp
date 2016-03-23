#include "include/Towers/Tower.h"

using namespace tower;

Tower::Tower(sf::Vector2f const &position, tinyxml2::XMLElement *xmlDef) :
Actor(xmlDef->FirstChildElement("Actor")),
mSecondsPerAttack(1.f / atof(xmlDef->FirstChildElement("AttacksPerSecond")->GetText())),
mSecondsSinceLastAttack(0.f),
mCost(atoi(xmlDef->FirstChildElement("Cost")->GetText()))
{
	setPosition(position);
	updateCollidableMask(getPosition());
}

Tower::~Tower() {
	//empty dtor body
}

void Tower::update(sf::Time const& elapsedTime) {
	mSecondsSinceLastAttack += elapsedTime.asSeconds();
}

int Tower::getCost() const {
	return mCost;
}
