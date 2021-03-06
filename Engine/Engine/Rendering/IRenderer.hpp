#ifndef _RENDERING_RENDERER_HPP
#define _RENDERING_RENDERER_HPP
#include "../General/Serialization.hpp"
#include "../General/Math.hpp"

class Camera;

class IRenderer {
public:
	virtual void Draw(const Camera& camera) = 0;
	
};

class ICanvasRenderer {
public:
	virtual void Draw(const vec2& screenSize) = 0;
	
};

#endif // !_RENDERING_RENDERER_HPP