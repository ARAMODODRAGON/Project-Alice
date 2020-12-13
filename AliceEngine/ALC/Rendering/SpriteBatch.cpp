#include "SpriteBatch.hpp"
#include <glew.h>
#include "../Content/ContentManager.hpp"
#include <stdexcept>

static constexpr const char* sprbatchShaderSrc[] = { R""(
#type vertex
#version 450 core
layout (location = 0) in vec2 a_position;
layout (location = 1) in vec2 a_uvcoords;
layout (location = 2) in vec4 a_color;
layout (location = 3) in int a_textureIndex;

uniform mat4 u_transform;

out vec4 v_color;
out vec2 v_uvcoords;
out float v_textureIndex;

void main() {

	v_color = a_color;
	v_textureIndex = float(a_textureIndex);
	v_uvcoords = a_uvcoords;
	vec4 vertex = u_transform * vec4(a_position, 0.0, 1.0);
	gl_Position = vertex;
	
}

#type fragment
#version 450 core
out vec4 out_fragcolor;

const int c_TextureCount = )"", R""(;
uniform sampler2D u_textures[c_TextureCount];

in vec4 v_color;
in vec2 v_uvcoords;
in float v_textureIndex;

void main() {

	if (int(v_textureIndex) == -1) {
		out_fragcolor = v_color;
	} else {
		out_fragcolor = texture(u_textures[int(v_textureIndex)], v_uvcoords) * v_color;
	} 
	
}

)"" };

namespace ALC {

	SpriteBatch::SpriteBatch()
		: m_vao(-1), m_vbo(-1), m_maxtextures(-1), m_TextureCountLoc(-1), m_bufferSize(0), m_camera(nullptr) {

		// get the max number of textures per shader
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_maxtextures);
		ALC_DEBUG_LOG("Max textures per batch: " + VTOS(m_maxtextures));
		if (m_maxtextures == -1) throw std::runtime_error("m_maxtextures was -1");

		// make sure our shader is loaded
		// set the max texture count
		string src = sprbatchShaderSrc[0] + VTOS(m_maxtextures) + sprbatchShaderSrc[1];
		m_defaultShader = ContentManager::LoadShaderSource(src);

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

	void SpriteBatch::Begin(Camera& camera, Shader shader) {
		m_textures.clear();
		m_verticies.clear();

		m_camera = &camera;

		// set the shader
		Shader currentShader = shader;
		if (currentShader == nullptr) currentShader = m_defaultShader;
		glUseProgram(currentShader);

		// bind vertex array and buffer
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		// set uniform data
		mat4 transform = m_camera->GetTransform();
		glUniformMatrix4fv(currentShader.GetUniform("u_transform"), 1, GL_FALSE, &(transform[0].x));

	}

	void SpriteBatch::Draw(const Transform& transform, const SpriteComponent& sprite) {

		// check if should batch break
		uint32 textureindex = TryAddTexture(sprite.texture);
		if (textureindex == -2) {
			DrawCurrent();
			m_textures.push_back(sprite.texture);
			textureindex = 0;
		}

		const vec2 position = vec2(transform.position) + sprite.offset;
		const vec2 min = position + sprite.bounds.min;
		const vec2 max = position + sprite.bounds.max;
		const vec2 size = sprite.texture.GetSize();
		const vec2 texmin = sprite.textureBounds.min / size;
		const vec2 texmax = sprite.textureBounds.max / size;

		// create verticies
		vertex verts[4];

		/* bottom left  */ verts[0].position = min;
		/* top left     */ verts[1].position = vec2(min.x, max.y);
		/* top right    */ verts[2].position = max;
		/* bottom right */ verts[3].position = vec2(max.x, min.y);

		// set color
		verts[0].color = verts[1].color
			= verts[2].color = verts[3].color = sprite.color;

		// set texture index
		verts[0].textureIndex = verts[1].textureIndex
			= verts[2].textureIndex = verts[3].textureIndex = textureindex;

		// set uvcoords
		/* bottom left  */ verts[0].uvcoords = vec2(texmin.x, texmax.y);
		/* top left     */ verts[1].uvcoords = texmin;
		/* top right    */ verts[2].uvcoords = vec2(texmax.x, texmin.y);
		/* bottom right */ verts[3].uvcoords = texmax;

		// push into vector
		m_verticies.push_back(verts[0]);
		m_verticies.push_back(verts[1]);
		m_verticies.push_back(verts[2]);
		m_verticies.push_back(verts[0]);
		m_verticies.push_back(verts[2]);
		m_verticies.push_back(verts[3]);

		// finish
	}

	void SpriteBatch::End() {

		// draw any remaining verticies
		DrawCurrent();

		// unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);

		// clean up
		m_camera = nullptr;
	}

	uint32 SpriteBatch::TryAddTexture(const Texture& texture) {
		// solid color
		if (texture == nullptr)
			return -1;

		// check if its already in the vector
		for (size_t i = 0; i < m_textures.size(); i++) {
			if (m_textures[i] == texture) {
				return i;
			}
		}

		// try add
		if (m_textures.size() < uint32(m_maxtextures)) {
			m_textures.push_back(texture);
			return m_textures.size() - 1;
		}

		return -2; // batch break
	}

	void SpriteBatch::DrawCurrent() {
		if (m_verticies.size() == 0)
			return;
		const uint32 bytes = (sizeof(vertex) * m_verticies.size());

		// resize the buffer if needed
		if (m_bufferSize < bytes) {
			if (m_bufferSize == 0) m_bufferSize = bytes;
			else m_bufferSize *= 2;
			glBufferData(GL_ARRAY_BUFFER, m_bufferSize, nullptr, GL_STREAM_DRAW);
		}

		// update the vertex data
		glBufferSubData(GL_ARRAY_BUFFER, 0, bytes, m_verticies.data());

		// load in the textures
		for (size_t i = 0; i < m_textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, m_textures[i]);
		}

		// draw
		glDrawArrays(GL_TRIANGLES, 0, m_verticies.size());

		// clear out vectors
		m_textures.clear();
		m_verticies.clear();
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