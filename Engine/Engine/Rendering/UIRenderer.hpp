#ifndef _RENDERING_UI_RENDERER_HPP
#define _RENDERING_UI_RENDERER_HPP
#include "../Objects/Object.hpp"
#include "IRenderer.hpp"

class UIRenderer : public Component, public ICanvasRenderer {


public:

	UIRenderer();
	~UIRenderer();

	// events
	void Start() override;
	void OnDestroy() override;


private:

	void Draw(const vec2& screenSize) override;

	RTTR_ENABLE(Component, ICanvasRenderer) RTTR_REGISTRATION_FRIEND
};

#endif // !_RENDERING_UI_RENDERER_HPP