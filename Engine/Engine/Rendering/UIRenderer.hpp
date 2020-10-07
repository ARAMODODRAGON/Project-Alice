#ifndef _RENDERING_UI_RENDERER_HPP
#define _RENDERING_UI_RENDERER_HPP
#include "../Objects/Object.hpp"
#include "IRenderer.hpp"
#include "ContentTypes.hpp"

class UIRenderer : public Component, public ICanvasRenderer {

	enum Element {
		Text,
		Rect,
	};

	struct DrawElement {
		vec3 color;
		string text;
		float x, y, width, height;
		float scale;
		char type;
	};

	vector<DrawElement> drawQueue;

	Font fontUI;
	Shader fontShader;
	unsigned int uniformColor, uniformScreenSize;
	unsigned int VAO, VBO;
public:

	UIRenderer();
	~UIRenderer();

	// events
	void Start() override;
	void OnDestroy() override;

	void DrawText(string text, float x, float y, float scale, vec3 color);
	void DrawRect(float x, float y, float width, float height, vec3 color);
private:

	void RenderText(DrawElement* element);
	void RenderRect(DrawElement* element);

	void Draw(const vec2& screenSize) override;

	RTTR_ENABLE(Component, ICanvasRenderer) RTTR_REGISTRATION_FRIEND
};

#endif // !_RENDERING_UI_RENDERER_HPP