#ifndef ALC_PHYSICS_TRANSFORM2D_HPP
#define ALC_PHYSICS_TRANSFORM2D_HPP
#include "../Entities/Registry.hpp"
#include <glm\glm.hpp>

namespace ALC {

	struct Transform2D final {

		vec2 position = vec2(0.0f);
		vec2 scale = vec2(0.0f);
		vec2 rotation = vec2(0.0f);

	};

}

#endif // !ALC_PHYSICS_TRANSFORM2D_HPP