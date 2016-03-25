#ifndef MAGE_TOWER_HPP
#define MAGE_TOWER_HPP

#include "BasicTower.h"

namespace tower {
	/*!
	\class MageTower
	\brief Tower that fires magical projectiles that home in on their target.
	*/
	class MageTower : public ProjectileTower {
	public:
		/*!
		\brief Constructs a Mage Tower.
		*/
		MageTower(sf::Vector2f const &position, tinyxml2::XMLElement *xmlDef);
		virtual ~MageTower();

		bool shoot(std::shared_ptr<std::list<std::shared_ptr<Pawn>>> const& possibleTargets) override;
	protected:

	};
}
#endif