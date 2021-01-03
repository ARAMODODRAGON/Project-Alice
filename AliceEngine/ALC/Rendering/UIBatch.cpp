#include "UIBatch.hpp"
#include <glew.h>
#include "detail\SpriteShaderSource.hpp"
#include "../SceneManager.hpp"

namespace ALC {

	UIBatch::UIBatch()
		: m_vao(-1), m_vbo(-1), m_TextureCountLoc(-1), m_bufferSize(0) {

		// get the max number of textures per shader
		GLint maxtextures = -1;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxtextures);

		// make sure our shader is loaded
		// set the max texture count
		m_defaultShader = detail::GetSpriteShader();

		// allocate the texture vector to match the max number
		m_textures.reserve(maxtextures);

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
		glVertexAttribIPointer(3, 1, GL_INT, sizeof(vertex), (GLvoid*)offsetof(vertex, textureIndex));

		// unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}

	UIBatch::~UIBatch() {
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
	}

	void UIBatch::Begin(Shader shader) {
		m_textures.clear();
		m_verticies.clear();

		// set the shader
		Shader currentShader = shader;
		if (currentShader == nullptr) currentShader = m_defaultShader;
		glUseProgram(currentShader);

		// bind vertex array and buffer
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		// set uniform data
		vec2 screensize = SceneManager::GetWindow()->GetScreenSize();
		mat4 transform = glm::ortho(0.0f, screensize.x, screensize.y, 0.0f);
		glUniformMatrix4fv(currentShader.GetUniform("u_transform"), 1, GL_FALSE, &(transform[0].x));
	}

	void UIBatch::DrawQuad(rect position, vec4 color, rect target, Texture texture) {

		// check if should batch break
		uint32 textureindex = TryAddTexture(texture);
		if (textureindex == -2) {
			DrawCurrent();
			m_textures.push_back(texture);
			textureindex = 0;
		}

		vec2 size = texture.GetSize();
		if (NearlyZero(size)) size = vec2(1.0f);

		// create verticies
		vertex verts[4];

		// set texture index
		verts[0].textureIndex = verts[1].textureIndex
			= verts[2].textureIndex = verts[3].textureIndex = textureindex;

		// set the positions
		/* bottom left  */ verts[0].position = position.min;
		/* top left     */ verts[1].position = vec2(position.left, position.top);
		/* top right    */ verts[2].position = position.max;
		/* bottom right */ verts[3].position = vec2(position.right, position.bottom);

		// set color
		verts[0].color = verts[1].color
			= verts[2].color = verts[3].color = color;

		// default uvs
		if (NearlyEqual(target.min, target.max)) {
			/* bottom left  */ verts[0].uvcoords = vec2(0.0f, 0.0f);
			/* top left     */ verts[1].uvcoords = vec2(0.0f, 1.0f);
			/* top right    */ verts[2].uvcoords = vec2(1.0f, 1.0f);
			/* bottom right */ verts[3].uvcoords = vec2(1.0f, 0.0f);
		}
		// given uvs
		else {
			/* bottom left  */ verts[0].uvcoords = target.min / size;
			/* top left     */ verts[1].uvcoords = vec2(target.left, target.top) / size;
			/* top right    */ verts[2].uvcoords = target.max / size;
			/* bottom right */ verts[3].uvcoords = vec2(target.right, target.bottom) / size;
		}

		// push into vector
		m_verticies.push_back(verts[0]);
		m_verticies.push_back(verts[1]);
		m_verticies.push_back(verts[2]);
		m_verticies.push_back(verts[0]);
		m_verticies.push_back(verts[2]);
		m_verticies.push_back(verts[3]);

		// finish
	}

	void UIBatch::End() {
		// draw any remaining verticies
		DrawCurrent();

		// unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}

	uint32 UIBatch::TryAddTexture(const Texture& texture) {
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
		if (m_textures.size() < m_textures.capacity()) {
			m_textures.push_back(texture);
			return m_textures.size() - 1;
		}

		return -2; // batch break
	}

	void UIBatch::DrawCurrent() {
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