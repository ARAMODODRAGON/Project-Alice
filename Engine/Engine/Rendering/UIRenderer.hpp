#ifndef _RENDERING_UI_RENDERER_HPP
#define _RENDERING_UI_RENDERER_HPP
#include "../Objects/Object.hpp"
#include "IRenderer.hpp"
#include "ContentTypes.hpp"

class UIRenderer : public Component, public ICanvasRenderer {

	enum class Element {
		None,		// A null element that will be ignored in the draw queue
		Text,		// A string being rendered using a specific font
		Quad,		// A quad that can have a texture on it
	};

	struct UIElement {
		unsigned int id;	// Stores the ID for thetexutre used by it
		string name;		// Name for the texture being used
		string text;		// String data for text
		vec3 color;			// Color (r, g, b)
		float x, y;			// Position Value
		float sx, sy;		// Scale Value
	};

	map<string, Font> fontUI;
	string currentFont;
	
	Shader fontShader;
	unsigned int uniformColor, uniformScreenSize;
	unsigned int VAO, VBO;

	vector<UIElement> drawQueue;
	unsigned int lastTexture;
public:

	UIRenderer();
	~UIRenderer();

	// events
	void Start() override;
	void OnDestroy() override;

	void AddFont(string fontName, float fontSize);
	void DrawSetFont(string fontName);
	void DrawText(string text, float x, float y, float sx, float sy, vec3 color);
private:

	void RenderText(UIElement* element);

	void Draw(const vec2& screenSize) override;

	RTTR_ENABLE(Component, ICanvasRenderer) RTTR_REGISTRATION_FRIEND
};

#endif // !_RENDERING_UI_RENDERER_HPP