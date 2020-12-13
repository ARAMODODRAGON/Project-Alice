#ifndef ALC_RENDERING_SPRITEBATCH_HPP
#define ALC_RENDERING_SPRITEBATCH_HPP
#include "../Entities/Registry.hpp"
#include "Camera.hpp"
#include "../Content/Texture.hpp"

namespace ALC {

	struct SpriteComponent final : ALC_Component {

		// the offset from the Entity's center
		vec2 offset;

		// the area in which to draw the sprite 
		// relative to the center + offset
		rect bounds;

		// the area of the texture to draw to screen
		rect textureBounds;

		// the texture to draw to screen
		Texture texture;

		// the color of the sprite,
		// if no texture then the sprite is a solid color
		vec4 color = ALC_COLOR_WHITE;

	};


	class SpriteBatch final {
		ALC_NO_COPY(SpriteBatch)
	public:

		SpriteBatch();
		~SpriteBatch();

		// begin drawing the sprites
		void Begin();

		// draw a sprite 
		void Draw(const Transform& transform, const SpriteComponent& sprite);

		// finish drawing the sprites
		void End(const Camera& camera /*, Shader shader */);

	private:

		struct vertex {
			vec2 position;
			vec2 uvcoords;
			vec4 color;
			int32 textureIndex = -1;
		};

		vector<vertex> m_verticies;
		vector<Texture> m_textures;
		uint32 m_vao, m_vbo;
		int32 m_maxtextures;
		uint32 m_textureLoc;

		bool TryAddTexture(const Texture& texture);
	};

}

#endif // !ALC_RENDERING_SPRITEBATCH_HPP