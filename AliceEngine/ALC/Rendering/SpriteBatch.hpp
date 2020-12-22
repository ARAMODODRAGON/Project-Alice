#ifndef ALC_RENDERING_SPRITEBATCH_HPP
#define ALC_RENDERING_SPRITEBATCH_HPP
#include "../Entities/Registry.hpp"
#include "Camera.hpp"
#include "../Content/Texture.hpp"
#include "../Content/Shader.hpp"
#include "../Physics/Transform2D.hpp"

namespace ALC {

	struct SpriteComponent final {

		// the offset from the Entity's center
		vec2 offset = vec2(0.0f);

		// the area in which to draw the sprite 
		// relative to the center + offset
		rect bounds;

		// the area of the texture to draw to screen
		// where 0,0 is the top left
		// and bottom left is the texture size in pixels
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
		void Begin(Camera& camera, Shader shader = nullptr);

		// draw a sprite 
		void Draw(const Transform2D& transform, const SpriteComponent& sprite);

		// finish drawing the sprites
		// uses the default shader if no shader is specified
		void End();

	private:

		struct vertex {
			vec2 position;
			vec2 uvcoords;
			vec4 color;
			int32 textureIndex = -1;
		};

		vector<vertex> m_verticies;
		vector<uint32> m_textures;
		uint32 m_vao, m_vbo;
		int32 m_maxtextures;
		uint32 m_TextureCountLoc;
		uint32 m_bufferSize;
		Camera* m_camera;
		Shader m_defaultShader;
		Shader m_currentShader;

		uint32 TryAddTexture(const Texture& texture);
		void DrawCurrent();
	};

}

#endif // !ALC_RENDERING_SPRITEBATCH_HPP