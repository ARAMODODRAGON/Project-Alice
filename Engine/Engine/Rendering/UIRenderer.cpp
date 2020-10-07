#include "UIRenderer.hpp"
#include "RenderScene.hpp"
#include <glew.h>

RTTR_REGISTRATION {
	registration::class_<UIRenderer>("UIRenderer")
		.public_object_constructor;
}

UIRenderer::UIRenderer() : VAO(-1), VBO(-1) {
	RenderScene::AddCanvasRenderer(this);

	// Reserve some memory for elements that are queued to draw to the screen
	drawQueue.reserve(10);

	// Load the font file
	fontUI = ContentHandler::LoadFont("TestFont", 80);

	// Fetch the shader used for rendering text to the screen
	fontShader = ContentHandler::LoadShader("font");
	uniformColor = glGetUniformLocation(fontShader, "textColor");
	uniformScreenSize = glGetUniformLocation(fontShader, "screensize");

	// Then, create the VAO and VBO used for the fonts
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	// vertex data is loaded dynamically for each and every character drawn to screen
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

UIRenderer::~UIRenderer() {
	RenderScene::RemoveCanvasRenderer(this);

	// delete vertex buffer
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void UIRenderer::Start() { }

void UIRenderer::OnDestroy() { }

void UIRenderer::DrawText(string text, float x, float y, float scale, vec3 color) {
	DrawElement element{};
	element.color = color;
	element.text = text;
	element.x = x;
	element.y = y;
	element.scale = scale;
	element.type = Element::Text;
	drawQueue.push_back(element);
}

void UIRenderer::DrawRect(float x, float y, float width, float height, vec3 color) {
	DrawElement element{};
	element.color = color;
	element.x = x;
	element.y = y;
	element.width = width;
	element.height = height;
	element.type = Element::Rect;
	drawQueue.push_back(element);
}

void UIRenderer::RenderText(DrawElement* element) {
	float scale = element->scale;
	vec3 color = element->color;
	glUniform3f(uniformColor, color.x, color.y, color.z);

	float xOffset = 0, yOffset = 0;
	float xPos, yPos, width, height;
	// Iterate through every character
	string::const_iterator c;
	for (c = element->text.begin(); c != element->text.end(); c++) {
		Character* chr = fontUI.GetCharacter(*c);

		// Get the x and y position for the current glyph
		xPos = element->x + xOffset + chr->bearing.x * scale;
		yPos = element->y + yOffset - (chr->size.y - chr->bearing.y) * scale;
		// Stores the size of the glyph on the screen
		width = chr->size.x * scale;
		height = chr->size.y * scale;
		// Update the VBO for each character
		float vertices[6][4] = {
			{ xPos,			yPos + height,	0.0f, 0.0f },
			{ xPos,			yPos,			0.0f, 1.0f },
			{ xPos + width, yPos,			1.0f, 1.0f },

			{ xPos,			yPos + height,	0.0f, 0.0f },
			{ xPos + width, yPos,			1.0f, 1.0f },
			{ xPos + width, yPos + height,	1.0f, 0.0f },
		};
		// Render glyph texture over the quad
		glBindTexture(GL_TEXTURE_2D, chr->textureID);
		// Update content of the VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render the quad to the screen
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for the next glyph (Note that the advance is the number of 1/64 pixels)
		xOffset += (chr->advance >> 6) * scale;
	}
}

void UIRenderer::RenderRect(DrawElement* element) {
	
}

void UIRenderer::Draw(const vec2& screenSize) {
	glEnable(GL_BLEND);
	glUseProgram(fontShader);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	glUniform2f(uniformScreenSize, screenSize.x, screenSize.y);

	for (auto element : drawQueue) {
		switch (element.type) {
			case Element::Text:
				RenderText(&element);
				break;
			case Element::Rect:
				RenderRect(&element);
				break;
		}
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	// Finally, clear the draw queue for the next frame
	drawQueue.clear();
}
