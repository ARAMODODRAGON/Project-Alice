#include "Font.hpp"
#include <stdexcept>
#include "../SceneManager.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glew.h>

namespace ALC {

	Font::Font() : m_textureID(0), m_textureSize(0), m_characters(nullptr) { }

	Font::Font(std::nullptr_t) : Font() { }

	bool Font::IsValid() const {
		return m_textureID != 0;
	}

	Font::operator uint32() const {
		return m_textureID;
	}

	Texture Font::GetTexture() const {
		return Texture(m_textureID, m_textureSize);
	}

	uint32 Font::GetID() const {
		return m_textureID;
	}

	uvec2 Font::GetSize() const {
		return m_textureSize;
	}

	bool Font::Contains(const char c) const {
		if (m_characters.get() == nullptr)
			return false;
		return m_characters.get()->find(c) != m_characters.get()->end();
	}

	const Font::Character& Font::At(const char c) const {
		if (m_characters.get() == nullptr)
			throw std::out_of_range("map was null");
		return m_characters.get()->at(c);
	}

	const Font::Character& Font::operator[](const char c) const {
		return m_characters.get()->operator[](c);
	}

	size_t Font::Size() const {
		if (m_characters.get() == nullptr)
			return 0;
		return m_characters->size();
	}

	bool Font::operator==(const Font& other) const {
		return m_textureID == other.m_textureID;
	}

	bool Font::operator!=(const Font& other) const {
		return m_textureID != other.m_textureID;
	}

	Font Font::Load(const string& path, const uint32 size) {
		// get the library
		FT_Library lib = SceneManager::__GetFTLibrary();

		// create a face
		FT_Face face;
		if (FT_New_Face(lib, path.c_str(), 0, &face)) {
			ALC_DEBUG_ERROR("Failed to load the font at path " + path);
			return nullptr;
		}

		// dunno what this is about
		FT_Set_Pixel_Sizes(face, 0, size);

		// get the size of the texture atlas that the font will be stored on
		FT_GlyphSlot g = face->glyph;
		int w = 0, h = 0;
		for (int i = 32; i < 128; i++) {
			if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
				ALC_DEBUG_ERROR("Failed to load glyph at index: " + VTOS(i));
				continue;
			}
			w += g->bitmap.width;
			if (h < g->bitmap.rows)
				h = g->bitmap.rows;
		}

		// create our texture
		unsigned int textureID = -1;
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		Ref<unordered_map<char, Character>> characters(new unordered_map<char, Character>());
		int x = 0;
		for (int i = 32; i < 128; i++) {
			if (FT_Load_Char(face, i, FT_LOAD_RENDER)) { // Skip over the elements that aren't properly loaded
				continue;
			}
			glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

			Character character{};
			character.advance.x = g->advance.x >> 6;
			character.advance.y = g->advance.y >> 6;
			character.bitSize.x = g->bitmap.width;
			character.bitSize.y = g->bitmap.rows;
			character.position.x = g->bitmap_left;
			character.position.y = g->bitmap_top;
			character.xoffset = (float)x / w;

			characters->insert(pair<char, Character>(i, character));
			x += g->bitmap.width;
		}

		// cleanup
		glPixelStorei(GL_UNPACK_ALIGNMENT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		FT_Done_Face(face);

		// create and return the font
		Font font;
		font.m_textureSize = uvec2(w, h);
		font.m_textureID = textureID;
		font.m_characters = std::move(characters);
		return font;
	}

	void Font::Delete(const Font& font) {

	}

}