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
		float value;
		float grant() { if (value > IMMUNE) value -= 0.25f; return value; }
		float strip() { if (value < NONE) value += 0.25f; return value; }

		explicit Reduction(const float _value);

		float operator*(float x) const;
		friend float operator*=(float num, Reduction &red);
		Reduction& operator=(float newValue);
	};

	/*!
	\brief Types of damage. Used when calculating resistance to damage.
	*/
	enum Type {
		PHYSICAL, MAGICAL
	};
};
#endif