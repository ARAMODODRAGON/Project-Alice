#include "Texture.hpp"
#include <glew.h>
#define STBI_NO_GIF // we dont want any gif loading
#define STB_IMAGE_IMPLEMENTATION
#include "detail\stb_image.h"

namespace ALC {

	Texture::Texture() : m_textureID(-1), m_textureSize(0) { }

	Texture::Texture(std::nullptr_t) : Texture() { }

	bool Texture::IsValid() const {
		return m_textureID != -1;
	}

	Texture::operator uint32() const {
		return m_textureID;
	}

	uint32 Texture::GetID() const {
		return m_textureID;
	}

	uvec2 Texture::GetSize() const {
		return m_textureSize;
	}

	bool Texture::operator==(const Texture& other) const {
		return m_textureID == other.m_textureID;
	}

	bool Texture::operator!=(const Texture& other) const {
		return m_textureID != other.m_textureID;
	}

	Texture Texture::Load(const string& path) {

		// load the texture data
		int width, height, channels;
		stbi_uc* pixels = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

		// failed to load
		if (pixels == nullptr) return nullptr;

		// create texture ID
		uint32 textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		//uint32 mode = 0;
		// set the texture mode
		//if (channels == STBI_rgb_alpha)
		//	mode = GL_RGBA;
		//else if (channels == STBI_rgb)
		//	mode = GL_RGB;
		//else if (channels == STBI_grey_alpha)
		//	mode = GL_RG;
		//else if (channels == STBI_grey)
		//	mode = GL_R;
		//// invalid? dunno if this really will do anything
		//else {
		//	ALC_DEBUG_ERROR("Invalid image channel mode when loading " + path);
		//	stbi_image_free(pixels);
		//	return nullptr;
		//}

		// load in and then free the texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		stbi_image_free(pixels);

		// Wrapping and filtering options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// unbind
		glBindTexture(GL_TEXTURE_2D, 0);

		// create the texture object and then return
		Texture texture;
		texture.m_textureID = textureID;
		texture.m_textureSize = uvec2(width, height);
		return texture;
	}

	void Texture::Delete(const Texture& texture) {
		// delete the texture
		glDeleteTextures(1, &texture.m_textureID);
	}

}