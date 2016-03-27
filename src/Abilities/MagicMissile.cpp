#include <include/Abilities/MagicMissileAbility.hpp>

using namespace abilities;

MagicMisile::MagicMisile(tinyxml2::XMLElement* xml) :
Ability(xml),
mCastGraphics(xml->FirstChildElement("Actor"))
{
}

MagicMisile::~MagicMisile() {
}

void MagicMisile::execute(Pawn* user) {
}

void MagicMisile::doUpdateLogic(float deltaSeconds) {
}

void MagicMisile::draw(sf::RenderTarget& target, sf::RenderStates states) const {
}