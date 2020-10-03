#ifndef _RENDERING_UI_RENDERER_HPP
#define _RENDERING_UI_RENDERER_HPP
#include "../Objects/Object.hpp"
#include "Tools/IRenderer.hpp"

class UIRenderer : public Component, public ICanvasRenderer {


public:

	UIRenderer();
	~UIRenderer();

	// events
	void Start() override;
	void OnDestroy() override;


private:

	virtual void UpdateUniforms(Shader shader) override;
	void Draw() override;

	RTTR_ENABLE(Component, ICanvasRenderer) RTTR_REGISTRATION_FRIEND
};

#endif // !_RENDERING_UI_RENDERER_HPP