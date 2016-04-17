#ifndef _DAMAGE_H
#define _DAMAGE_H
namespace Damage {
	/*!
	\brief Damage Reduction, such as Armour or Magic Resistance.
	Used to reduce damage by an appropriate amount.
	Usage: Damage::Reduction mArmour = Damage::Reduction::MEDIUM;
	\see Pawn::takeDamage().
	*/
	struct Reduction {
		static const float NONE, LIGHT, MEDIUM, HEAVY, IMMUNE;

		//! Damage reduction multiplier.
		float value;

		//!Grants a level of reduction if below the max.
		float grant() { if (value > IMMUNE) value -= 0.25f; return value; }

		//!Strips a level of reduction if above none.
		float strip() { if (value < NONE) value += 0.25f; return value; }

		explicit Reduction(const float _value);

		//!Applies the reduction to the float passed.
		float operator*(float damage) const;

		//!Applies the reduction to the float passed.
		friend float operator*=(float num, Reduction &red);

		//! Assigns a new reduction value.
		Reduction& operator=(float newValue);

		//! Returns true if we have a lower value.
		bool operator<(Reduction const& other) const;

		//! Returns true if we have a lower or equal value.
		bool operator<=(Reduction const& other) const;
	};

	/*!
	\brief Types of damage. Used when calculating resistance to damage.
	*/
	enum Type {
		PHYSICAL, MAGICAL
	};
};
#endif