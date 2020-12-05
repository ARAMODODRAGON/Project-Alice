#ifndef ALC_ENTITIES_COMPONENTS_HPP
#define ALC_ENTITIES_COMPONENTS_HPP
#include "../General.hpp"
#include "EntityRegistry.hpp"

namespace ALC {

	struct Transform2D ALC_COMPONENT {
		vec2 position;
		vec2 scale;
		float rotation = 0.0f;
	};

}

#endif // !ALC_ENTITIES_COMPONENTS_HPP