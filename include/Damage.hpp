#pragma once
namespace Damage {
	struct Reduction {
		static const float NONE, LIGHT, MEDIUM, HEAVY, IMMUNE;
		float value;
		float grant() { if (value > IMMUNE) value -= 0.25f; return value; }
		float strip() { if (value < NONE) value += 0.25f; return value; }
		//Usage: Damage::Reduction mArmour = { Damage::Reduction::MEDIUUM };

		float operator*(float x) {
			return value * x;
		}
		friend float operator*=(float num, Reduction &red) {
			return red * num;
		}
	};

	enum Type {
		PHYSICAL, MAGICAL
	};
};