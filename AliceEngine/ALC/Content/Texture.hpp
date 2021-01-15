#ifndef ALC_CONTENT_TEXTURE_HPP
#define ALC_CONTENT_TEXTURE_HPP
#include "../General.hpp"

namespace ALC {

	struct Texture final {

		// default constructor
		Texture();

		// de initializes the texture
		Texture(std::nullptr_t);

		// initializes with the given id and size
		Texture(const uint32 textureID, const uvec2& textureSize);

		// returns true if this is a valid texture
		bool IsValid() const;

		// returns the texture ID
		operator uint32() const;

		// returns the texture ID
		uint32 GetID() const;

		// returns the texture size
		uvec2 GetSize() const;

		// returns a rect with the min set to 0,0 and max set to GetSize()
		rect GetBounds() const;

		// compare the textures
		bool operator==(const Texture& other) const;

		// compare the textures
		bool operator!=(const Texture& other) const;

		// functions for loading and deleting textures

		// loads a texture based on the path
		static Texture Load(const string& path);

		// deletes a texture
		static void Delete(const Texture& texture);

	private:
		uint32 m_textureID;
		uvec2 m_textureSize;
	};

}

#endif // !ALC_CONTENT_TEXTURE_HPP