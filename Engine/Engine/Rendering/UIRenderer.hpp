#ifndef _RENDERING_UI_RENDERER_HPP
#define _RENDERING_UI_RENDERER_HPP
#include "../Objects/Object.hpp"
#include "IRenderer.hpp"
#include "ContentTypes.hpp"

class UIRenderer : public Component, public ICanvasRenderer {

	struct UIElement {
		string text;	// String data for text
		vec3 color;		// Color (r, g, b)
		float x, y;		// Position Value
		float sx, sy;	// Scale Value
	};
	vector<UIElement> drawQueue;

	Font fontUI;
	Shader UIShader;
	unsigned int uniformColor, uniformScreenSize;
	unsigned int VAO, VBO;
public:

	UIRenderer();
	~UIRenderer();

	// events
	void Start() override;
	void OnDestroy() override;

	void DrawText(string text, float x, float y, float sx, float sy, vec3 color);
private:

	void RenderText(UIElement* element);

	void Draw(const vec2& screenSize) override;

	RTTR_ENABLE(Component, ICanvasRenderer) RTTR_REGISTRATION_FRIEND
};

#endif // !_RENDERING_UI_RENDERER_HPP