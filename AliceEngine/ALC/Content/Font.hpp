#ifndef ALC_CONTENT_FONT_HPP
#define ALC_CONTENT_FONT_HPP
#include "../General.hpp"

namespace ALC {

	struct Font final {

		// struct containing character data
		struct Character final {
			vec2 advance;
			vec2 bitSize;
			vec2 position;
			float xoffset;
		};

		// default constructor
		Font();

		// de initializes the font
		Font(std::nullptr_t);

		// returns true if this is a valid font
		bool IsValid() const;

		// returns the texture ID
		operator uint32() const;

		// returns the texture ID
		uint32 GetID() const;

		// returns the texture size
		uvec2 GetSize() const;

		// checks if it contains the given character
		bool Contains(const char c) const;

		// returns the character
		// if not found, throws std::out_of_range
		const Character& At(const char c) const;

		// returns the character
		const Character& operator[](const char c) const;

		// compare the fonts
		bool operator==(const Font& other) const;

		// compare the fonts
		bool operator!=(const Font& other) const;

		// functions for loading and deleting fonts

		// loads a font based on the path
		static Font Load(const string& path, const uint32 size);

		// deletes a font
		static void Delete(const Font& font);

	private:
		uint32 m_textureID;
		uvec2 m_textureSize;
		Ref<unordered_map<char, Character>> m_characters;
	};

}

#endif // !ALC_CONTENT_FONT_HPP