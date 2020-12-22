#include "Font.hpp"
#include <stdexcept>
#include <ft2build.h>
#include <freetype\freetype.h>

namespace ALC {

	Font::Font() : m_textureID(0), m_textureSize(0), m_characters(nullptr) { }

	Font::Font(std::nullptr_t) : Font() { }

	bool Font::IsValid() const {
		return m_textureID != 0;
	}

	Font::operator uint32() const {
		return m_textureID;
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

	bool Font::operator==(const Font& other) const {
		return m_textureID == other.m_textureID;
	}

	bool Font::operator!=(const Font& other) const {
		return m_textureID != other.m_textureID;
	}

	Font Font::Load(const string& path, const uint32 size) {
		ALC_DEBUG_ERROR("Font loading not implemented");
		return Font();
	}

	void Font::Delete(const Font& font) { 
		ALC_DEBUG_ERROR("Font deleting not implemented");
	}

}