#ifndef ALC_PHYSICS_RIGIDBODY2D_HPP
#define ALC_PHYSICS_RIGIDBODY2D_HPP
#include "Transform2D.hpp"

namespace ALC {

	struct Rigidbody2D : ALC_Component {

		glm::vec2 velocity;
		glm::vec2 acceleration;

	};

}

#endif // !ALC_PHYSICS_RIGIDBODY2D_HPP