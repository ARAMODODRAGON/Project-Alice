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
	fontShader = ContentHandler::LoadShader("font");
	// Getting the vertex uniforms; storing them in local variables
	uniformScreenSize = glGetUniformLocation(fontShader, "screenSize");
	// Getting the fragment uniforms; storing them in local variables
	uniformColor = glGetUniformLocation(fontShader, "textColor");

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

	AddFont("TestFont", 40);
	AddFont("TestFont2", 40);
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
	drawQueue.push_back(element);
}

void UIRenderer::RenderText(UIElement* element) {
	// Don't draw any text if no font is currently active
	if (currentFont == "") { return; }
	Font curFont = fontUI[element->name];
	// Before rendering anything, check if the current texture ID is equal to the current font's texture ID
	if (lastTexture != curFont.GetAtlasID() || lastTexture == -1) {
		glBindTexture(GL_TEXTURE_2D, curFont.GetAtlasID());
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
		coords.push_back(vec4(x2, -y2, c->tx, 0.0f));
		coords.push_back(vec4(x2 + w, -y2, c->tx + (c->bw / width), 0.0f));
		coords.push_back(vec4(x2, -y2 - h, c->tx, c->bh / height));
		coords.push_back(vec4(x2 + w, -y2, c->tx + (c->bw / width), 0.0f));
		coords.push_back(vec4(x2, -y2 - h, c->tx, c->bh / height));
		coords.push_back(vec4(x2 + w, -y2 - h, c->tx + (c->bw / width), c->bh / height));
	}

	// Finally, set the color used within the shader and draw all the coordinated
	vec3 color = element->color;
	glUniform3f(uniformColor, color.x, color.y, color.z);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * coords.size(), coords.data(), GL_DYNAMIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, coords.size());
}

void UIRenderer::Draw(const vec2& screenSize) {
	glEnable(GL_BLEND);
	glUseProgram(fontShader);
	glUniform2f(uniformScreenSize, screenSize.x, screenSize.y);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glActiveTexture(GL_TEXTURE0);

	DrawSetFont("TestFont2");
	DrawText("The quick brown fox jumps over the lazy dog", -screenSize.x, 100.0, 1.0, 1.0, vec3(1.0));
	DrawSetFont("TestFont");
	DrawText("THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG", -screenSize.x, 200.0, 1.0, 1.0, vec3(0.0, 0.0, 1.0));

	for (auto element : drawQueue) {
		RenderText(&element);

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
