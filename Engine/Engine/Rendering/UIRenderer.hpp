#ifndef _RENDERING_UI_RENDERER_HPP
#define _RENDERING_UI_RENDERER_HPP
#include "../Objects/Object.hpp"
#include "IRenderer.hpp"
#include "ContentTypes.hpp"

class UIRenderer : public Component, public ICanvasRenderer {

	enum class Element : int {
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
		Element type;		// The element contained within this struct
	};

	// Map of available fonts to use when drawing text and the currently active one's key
	map<string, Font> fontUI;
	string currentFont;
	
	// Map of available textures that have been added to the UI renderer
	map<string, Texture> spriteUI;
	
	// Stores the ID for the shader used for rendering GUI elements. Below that is the uniforms used within
	// the shader itself, and the VAO and VBO that contain vertice data for the GPU to utilize.
	Shader UIShader;
	unsigned int uniformTextureType, uniformColor, uniformScreenSize;
	unsigned int VAO, VBO;

	// The vector to store the elements that will be drawn to the screen, which can be text, sprites, etc.
	// Below that is the ID for the texture used by the previously drawn element.
	vector<UIElement> drawQueue;
	unsigned int lastTexture;
public:
	// Constructor/Destructor
	UIRenderer();
	~UIRenderer();

	// Events
	void Start() override;
	void OnDestroy() override;

	// Font functions
	void AddFont(string fontName, float fontSize);
	void DrawSetFont(string fontName);
	void DrawText(string text, float x, float y, float sx, float sy, vec3 color);
	// Sprite/texture functions
	void AddSprite(string textureName);
	void DrawSprite(string textureName, float x, float y, float sx, float sy);
private:
	// Functions that actually handle passing the vertices to the GPU
	void RenderText(UIElement* element);
	void RenderSprite(UIElement* element);

	// Renders each GUI element in order to the screen
	void Draw(const vec2& screenSize) override;

};

#endif // !_RENDERING_UI_RENDERER_HPP