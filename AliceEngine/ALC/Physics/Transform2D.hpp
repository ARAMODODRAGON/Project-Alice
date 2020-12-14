#ifndef ALC_PHYSICS_TRANSFORM2D_HPP
#define ALC_PHYSICS_TRANSFORM2D_HPP
#include "../Entities/Registry.hpp"
#include <glm\glm.hpp>

namespace ALC {

	struct Transform2D final : ALC_Component {

		glm::vec2 position;
		glm::vec2 scale;
		glm::vec2 rotation;

	};

}

#endif // !ALC_PHYSICS_TRANSFORM2D_HPP