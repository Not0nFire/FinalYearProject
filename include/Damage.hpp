#ifndef _DAMAGE_H
#define _DAMAGE_H
namespace Damage {
	struct Reduction {
		static const float NONE, LIGHT, MEDIUM, HEAVY, IMMUNE;
		float value;
		float grant() { if (value > IMMUNE) value -= 0.25f; return value; }
		float strip() { if (value < NONE) value += 0.25f; return value; }
		//Usage: Damage::Reduction mArmour = { Damage::Reduction::MEDIUUM };

		explicit Reduction(const float _value);

		float operator*(float x);
		friend float operator*=(float num, Reduction &red);
	};

	enum Type {
		PHYSICAL, MAGICAL
	};
};
#endif