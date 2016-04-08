#ifndef HEAL_ABILITY_HPP
#define HEAL_ABILITY_HPP

#include "Ability.hpp"
#include <Thor/Particles.hpp>

namespace abilities
{
	class Heal : public Ability {
	public:
		explicit Heal(tinyxml2::XMLElement* xml);
		~Heal();

	protected:
		void doExecuteLogic(Pawn* user) override;
		void doUpdateLogic(sf::Time const& deltaTime) override;
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	private:
		Actor mCastGraphics;
		const int mHealAmount;
		thor::ParticleSystem mParticleSystem;
		thor::UniversalEmitter mEmitter;
	};
}
#endif