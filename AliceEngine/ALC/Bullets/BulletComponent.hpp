#ifndef ALC_BULLETS_COMPONENT_HPP
#define ALC_BULLETS_COMPONENT_HPP
#include "../General.hpp"

namespace ALC {

	struct BulletComponent final {

		bool isAlive = true;

		// the bullets lifetime
		// counts down until zero and then the bullet dies
		// negative means that the bullet never dies
		float lifetime = -1.0f;

		// the damage output of the bullet
		// zero or negative makes the bullet intangible
		float damage = 0.0f;

		// requires the BaseBulletSystem be attached
		float speedMult = 1.3f;
		float speedReduction = 0.1f;
		vec2 realVelocity = vec2(0.0f);
		uint8 basebulletState = 0;

		// state, can be used for anything
		uint32 state = 0;

	};

}

#endif // !ALC_BULLETS_COMPONENT_HPP