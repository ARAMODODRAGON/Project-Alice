#ifndef ALC_PHYSICS_RIGIDBODY2D_HPP
#define ALC_PHYSICS_RIGIDBODY2D_HPP
#include "Transform2D.hpp"

namespace ALC {

	struct Rigidbody2D final {

		vec2 velocity = vec2(0.0f);
		vec2 acceleration = vec2(0.0f);

	};

}

#endif // !ALC_PHYSICS_RIGIDBODY2D_HPP