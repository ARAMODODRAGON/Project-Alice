#ifndef _RENDERING_RENDERER_HPP
#define _RENDERING_RENDERER_HPP
#include "../General/Serialization.hpp"

class Camera;

class IRenderer {
public:
	virtual void Draw(const Camera& camera) = 0;
	RTTR_ENABLE()
};

#endif // !_RENDERING_RENDERER_HPP