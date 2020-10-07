#include "UIRenderer.hpp"
#include "RenderScene.hpp"
#include <glew.h>

RTTR_REGISTRATION {
	registration::class_<UIRenderer>("UIRenderer")
		.public_object_constructor;
}

UIRenderer::UIRenderer() {
	RenderScene::AddCanvasRenderer(this);

	// Reserve some memory for elements that are queued to draw to the screen
	//drawQueue.reserve(10);

	// Load the font file
	fontUI = ContentHandler::LoadFont("TestFont", 42);

	// Fetch the shader used for rendering text to the screen
	fontShader = ContentHandler::LoadShader("font");
	uniformColor = glGetUniformLocation(fontShader, "textColor");
	uniformScreenSize = glGetUniformLocation(fontShader, "screensize");

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 4, (GLvoid*)0);
}

UIRenderer::~UIRenderer() {
	RenderScene::RemoveCanvasRenderer(this);
}

void UIRenderer::Start() { }

void UIRenderer::OnDestroy() { }

void UIRenderer::DrawText(string text, float x, float y, float sx, float sy, vec3 color) {
	vector<vec4> coords;
	for (const char* p = text.c_str(); *p; p++) {
		Character* c = fontUI.GetCharacter(*p);
		float x2 = x + c->bl * sx;
		float y2 = -y - c->bt * sy;
		float w = c->bw * sx;
		float h = c->bh * sy;

		// Advance the cursor to the start of the next character
		x += c->ax * sx;
		y += c->ay * sy;

		// Skip glyphs that don't contain any pixels
		if (!w || !h) {
			continue;
		}

		float width = fontUI.GetAtlasWidth();
		float height = fontUI.GetAtlasHeight();
		coords.push_back(vec4(x2,		-y2,		c->tx,						0.0f));
		coords.push_back(vec4(x2 + w,	-y2,		c->tx + (c->bw / width),	0.0f));
		coords.push_back(vec4(x2,		-y2 - h,	c->tx,						c->bh / height));
		coords.push_back(vec4(x2 + w,	-y2,		c->tx + (c->bw / width),	0.0f));
		coords.push_back(vec4(x2,		-y2 - h,	c->tx,						c->bh / height));
		coords.push_back(vec4(x2 + w,	-y2 - h,	c->tx + (c->bw / width),	c->bh / height));
	}

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
	glBindTexture(GL_TEXTURE_2D, fontUI.GetAtlasID());
	glActiveTexture(GL_TEXTURE0);

	DrawText("!\"#$%&'+-*().,0123456789?@`{}|`", -screenSize.x, 0.0, 1.0, 1.0, vec3(0.5, 0.5, 0.5));
	DrawText("The quick brown fox jumps over the lazy dog.", -screenSize.x, 100.0, 1.0, 1.0, vec3(1.0));
	DrawText("THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG!", -screenSize.x, 200.0, 1.0, 1.0, vec3(0.0, 0.0, 1.0));

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glUseProgram(0);
}
