#pragma once
struct Damage {
	//abstract struct (is there no better way of doing this?)
	virtual void abstract() = 0;

	struct Reduction {
		static const float NONE, LIGHT, MEDIUM, HEAVY, IMMUNE;
		float value;
		float grant() { if (value > IMMUNE) value -= 0.25f; return value; }
		float strip() { if (value < NONE) value += 0.25f; return value; }
		//Usage: Damage::Reduction mArmour = { Damage::Reduction::MEDIUUM };
	};

	enum Type {
		PHYSICAL, MAGICAL
	};
};