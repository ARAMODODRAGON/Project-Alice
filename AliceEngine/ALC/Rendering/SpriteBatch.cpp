#include "SpriteBatch.hpp"
#include <glew.h>
#include "../Content/ContentManager.hpp"

static constexpr const char* defaultSpriteShaderSource = R""(
#type vertex
#version 450 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 uvcoord;
layout (location = 2) in vec4 color;
layout (location = 3) in int textureIndex;

uniform mat4 transform;

out vec4 pass_color;
out vec2 pass_uvcoords;
out int pass_textureIndex;

void main() {

	pass_color = color;
	pass_textureIndex = textureIndex;
	pass_uvcoords = uvcoords;
	gl_Position = transform * vec4(position, 0.0, 1.0)

}

#type fragment
#version 450 core
out vec4 out_fragcolor;

uniform int TextureCount;
uniform sampler2D textures[TextureCount];

in vec4 pass_color;
in vec2 pass_uvcoords;
in int pass_textureIndex;

void main() {

	if (pass_textureIndex == -1)
		out_fragcolor = pass_color;
	else 
		out_fragcolor = texture(textures[pass_textureIndex], pass_uvcoords) * pass_color;
}

)"";

namespace ALC {

	SpriteBatch::SpriteBatch() 
		: m_vao(-1), m_vbo(-1), m_maxtextures(-1), m_TextureCountLoc(-1), m_camera(nullptr) {

		// make sure our shader is loaded
		m_defaultShader = ContentManager::LoadShaderSource(defaultSpriteShaderSource);

		// get the max number of textures per shader
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_maxtextures);

		// allocate the texture vector to match the max number
		m_textures.reserve(m_maxtextures);

		// allocate some memory for the vertex vector
		m_verticies.reserve(100);

		// create our VAO
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		// create our VBO
		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STREAM_DRAW);

		// set the attributes

		// set the position to location 0
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, position));

		// set the uvcoords to location 1
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, uvcoords));

		// set the color to location 2
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, color));

		// set the textureIndex to location 3
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 1, GL_INT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, textureIndex));

		// unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}

	SpriteBatch::~SpriteBatch() { }

	void SpriteBatch::Begin(Camera& m_camera, Shader shader = nullptr) {
		m_textures.clear();
		m_verticies.clear();
	}

	void SpriteBatch::Draw(const Transform& transform, const SpriteComponent& sprite) {

	}

	void SpriteBatch::End() {
		// set the shader
		if (m_currentShader == nullptr) glUseProgram(m_defaultShader);
		else glUseProgram(m_currentShader);

		// bind vertex array and buffer
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		// update the vertex data
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex) * 4 * m_verticies.size(), m_verticies.data());

		// load in the textures
		for (size_t i = 0; i < m_textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, m_textures[i]);
		}

		// set uniform data
		// TODO

		// draw
		glDrawArrays(GL_TRIANGLES, 0, m_verticies.size());

		// unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);

		// clean up
		m_currentShader = nullptr;
		m_camera = nullptr;
	}

	bool SpriteBatch::TryAddTexture(const Texture& texture) {
		return false;
	}

}

/*
// set the shader
if (shader == nullptr) shader = defaultShader;
glUseProgram(shader.GetID());

// bind vertex array and buffer
glBindVertexArray(m_vao);
glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

// update the vertex data
glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex) * 4 * m_verticies.size(), m_verticies.data());

// draw
glDrawArrays(GL_TRIANGLES, 0, m_verticies.size());

// unbind
glBindBuffer(GL_ARRAY_BUFFER, 0);
glBindVertexArray(0);
glUseProgram(0);
*/