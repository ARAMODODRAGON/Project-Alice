#include "UIRenderer.hpp"
#include "RenderScene.hpp"
#include <glew.h>

RTTR_REGISTRATION {
	registration::class_<UIRenderer>("UIRenderer")
		.public_object_constructor;
}

UIRenderer::UIRenderer() { 
	fontShader = NULL;
	VAO = -1;
	VBO = -1;
}

UIRenderer::~UIRenderer() { }

void UIRenderer::Start() {
	RenderScene::AddCanvasRenderer(this);
	// Load the font file
	fontUI = ContentHandler::LoadFont("TestFont");
	// Fetch the shader used for rendering text to the screen
	fontShader = ContentHandler::LoadShader("font");
	uniformColor = glGetUniformLocation(fontShader.GetID(), "textColor");
	// Then, create the VAO and VBO used for the fonts
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void UIRenderer::OnDestroy() {
	// remove
	RenderScene::RemoveCanvasRenderer(this);
}

void UIRenderer::DrawText(string text, float x, float y, float scale, vec3 color) {
	glUseProgram(fontShader);
	glUniform3f(uniformColor, color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	float xPos, yPos, width, height;
	// Iterate through every character
	string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		Character* chr = fontUI.GetCharacter(*c);

		// Get the x and y position for the current glyph
		xPos = x + chr->bearing.x * scale;
		yPos = y - (chr->size.y - chr->bearing.y) * scale;
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
		x += (chr->advance >> 6) * scale;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void UIRenderer::Draw(const vec2& screenSize) {
	
}
