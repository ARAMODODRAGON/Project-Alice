#include "UIRenderer.hpp"
#include "RenderScene.hpp"
#include <glew.h>

RTTR_REGISTRATION {
	registration::class_<UIRenderer>("UIRenderer")
		.public_object_constructor;
}

UIRenderer::UIRenderer() {
	RenderScene::AddCanvasRenderer(this);

	// Load the font file
	//fontUI = ContentHandler::LoadFont("TestFont", 42);

	// Store the shader used for rendering UI elements into a local variable
	UIShader = ContentHandler::LoadShader("font");
	// Getting the vertex uniforms; storing them in local variables
	uniformScreenSize = glGetUniformLocation(UIShader, "screenSize");
	// Getting the fragment uniforms; storing them in local variables
	uniformColor = glGetUniformLocation(UIShader, "textColor");

	// Gerenating the VAO and VBO that will be used for rendering text and primitives to the screen (Ex. Rectangles)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Bind the VBO to the VAO and fill it with nothing
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_DYNAMIC_DRAW);
	// Finally create an attribute pointer that will contain the position and texture coordinates of each fragment
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 4, (GLvoid*)0);

	// Initialize the currently active font key to ""
	currentFont = "";
	// Initialize the draw queue variables
	drawQueue.reserve(5);
	lastTexture = -1;

	AddFont("TestFont", 60);
	AddFont("TestFont2", 40);

	AddSprite("circle 128");
}

UIRenderer::~UIRenderer() {
	RenderScene::RemoveCanvasRenderer(this);
}

void UIRenderer::Start() { }

void UIRenderer::OnDestroy() { }

void UIRenderer::AddFont(string fontName, float fontSize) {
	Font newFont = ContentHandler::LoadFont(fontName, fontSize);
	fontUI.insert(pair<string, Font>(fontName, newFont));
	if (currentFont == "") { // By default, set the current font to the first added font
		currentFont = fontName;
	}
}

void UIRenderer::DrawSetFont(string fontName) {
	if (fontUI.find(fontName) != fontUI.end()) { // Make sure the font they've specified actually exists
		currentFont = fontName;
	} else { // Reset the currently used font back to "" if no valid font is found
		currentFont = "";
	}
}

void UIRenderer::DrawText(string text, float x, float y, float sx, float sy, vec3 color) {
	UIElement element{};
	element.id = fontUI[currentFont].GetAtlasID();
	element.name = currentFont;
	element.text = text;
	element.color = color;
	element.x = x;
	element.y = y;
	element.sx = sx;
	element.sy = sy;
	element.type = Element::Text;
	drawQueue.push_back(element);
}

void UIRenderer::RenderText(UIElement* element) {
	// Don't draw any text if no font is currently active
	if (currentFont == "") { return; }
	Font curFont = fontUI[element->name];
	// Before rendering anything, check if the current texture ID is equal to the current font's texture ID
	if (lastTexture != element->id || lastTexture == -1) {
		glBindTexture(GL_TEXTURE_2D, element->id);
	}

	// Loop through each character in the string and determine their position on the screen/texture coordinates
	float xOffset = 0, yOffset = 0;
	vector<vec4> coords;
	for (const char* p = element->text.c_str(); *p; p++) {
		Character* c = curFont.GetCharacter(*p);
		float x2 = element->x + xOffset + c->bl * element->sx;
		float y2 = -element->y - yOffset - c->bt * element->sy;
		float w = c->bw * element->sx;
		float h = c->bh * element->sy;

		// Advance the cursor to the start of the next character
		xOffset += c->ax * element->sx;
		yOffset += c->ay * element->sy;

		// Skip glyphs that don't contain any pixels
		if (!w || !h) {
			continue;
		}

		float width = curFont.GetAtlasWidth();
		float height = curFont.GetAtlasHeight();
		coords.push_back(vec4(x2,		-y2,		c->tx,						0.0f));
		coords.push_back(vec4(x2 + w,	-y2,		c->tx + (c->bw / width),	0.0f));
		coords.push_back(vec4(x2,		-y2 - h,	c->tx,						c->bh / height));
		coords.push_back(vec4(x2 + w,	-y2,		c->tx + (c->bw / width),	0.0f));
		coords.push_back(vec4(x2,		-y2 - h,	c->tx,						c->bh / height));
		coords.push_back(vec4(x2 + w,	-y2 - h,	c->tx + (c->bw / width),	c->bh / height));
	}

	// Finally, set the color used within the shader and draw all the coordinated
	vec3 color = element->color;
	glUniform3f(uniformColor, color.x, color.y, color.z);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * coords.size(), coords.data(), GL_DYNAMIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, coords.size());
}

void UIRenderer::AddSprite(string textureName) {
	Texture newTexture = ContentHandler::LoadTexture(textureName);
	spriteUI.insert(pair<string, Texture>(textureName, newTexture));
}

void UIRenderer::DrawSprite(string textureName, float x, float y, float sx, float sy) {
	UIElement element{};
	element.id = spriteUI[textureName].GetID();
	element.name = textureName;
	element.x = x;
	element.y = y;
	element.sx = sx;
	element.sy = sy;
	element.type = Element::Quad;
	drawQueue.push_back(element);
}

void UIRenderer::RenderSprite(UIElement* element) {
	// Before rendering anything, check if the current texture ID is equal to the current sprite's texture ID
	if (lastTexture != element->id) {
		glBindTexture(GL_TEXTURE_2D, element->id);
	}
	Texture curTexture = spriteUI[element->name];
	vec2 size = curTexture.GetSize() * vec2(element->sx, element->sy);

	vec4 coords[6];
	coords[0] = vec4(element->x,			-element->y,				0.0f, 0.0f);
	coords[1] = vec4(element->x + size.x,	-element->y,				1.0f, 0.0f);
	coords[2] = vec4(element->x,			-element->y - size.y,		0.0f, 1.0f);
	coords[3] = vec4(element->x + size.x,	-element->y,				1.0f, 0.0f);
	coords[4] = vec4(element->x,			-element->y - size.y,		0.0f, 1.0f);
	coords[5] = vec4(element->x + size.x,	-element->y - size.y,		1.0f, 1.0f);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * 6, coords, GL_DYNAMIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void UIRenderer::Draw(const vec2& screenSize) {
	glEnable(GL_BLEND);
	glUseProgram(UIShader);
	glUniform2f(uniformScreenSize, screenSize.x, screenSize.y);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glActiveTexture(GL_TEXTURE0);

	DrawSetFont("TestFont2");
	DrawText("The quick brown fox jumps over the lazy dog", -screenSize.x, 100.0, 1.0, 1.0, vec3(1.0));
	DrawSetFont("TestFont");
	DrawText("THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG", -screenSize.x, 200.0, 1.0, 1.0, vec3(0.0, 0.0, 1.0));

	DrawSprite("circle 128", 0.0, 0.0, 2.5, 2.5);

	// Loop through every element on the draw queue and draw them in order
	for (auto element : drawQueue) {
		switch (element.type) {
			case Element::Text: // Draws the element as text to the screen
				RenderText(&element);
				break;
			case Element::Quad: // Draws the element as a single quad with a texture applied to it
				RenderSprite(&element);
				break;
			default: // Throw an error message for broken elements
				DEBUG_WARNING("Element doesn't have a valid type!");
				break;
		}
		lastTexture = element.id;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glUseProgram(0);

	// Empty the vector and reset the previously used texture ID for the next frame
	drawQueue.clear();
	lastTexture = -1;
}
