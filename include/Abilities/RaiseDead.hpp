#ifndef RAISE_DEAD_ABILITY_HPP
#define RAISE_DEAD_ABILITY_HPP

#include "Ability.hpp"
#include <include/Minion.hpp>

namespace abilities
{
	class RaiseDead : public Ability {
	public:
		RaiseDead(tinyxml2::XMLElement* xml);
		virtual ~RaiseDead();
		void execute(Pawn* user) override;
	protected:
		void doUpdateLogic(float deltaSeconds) override;
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	private:
		Minion mUnitPrototype;
		std::vector<sf::Vector2f const> mSpawnPoints;
	};
}
#endif