#ifndef ALC_2D_TILEMAP_HPP
#define ALC_2D_TILEMAP_HPP
#include "../General.hpp"
#include "../Rendering/SpriteBatch.hpp"
#include "../Layermask.hpp"

namespace ALC {

	class Tilemap final {
	public:

		struct Tile final {
			// the id that represents the tile 
			uint32 id;
			// a general purpose 32 bit layermask
			Layermask32 mask;
		};

		Tilemap();
		~Tilemap();

		// loads a json file containing the tilemap data
		bool Load(const string& path);

		// saves this tilemap to a file. creates one if it doesnt exist
		bool Save(const string& path);

		// the position of the tilemap itself
		// the [0, 0] tile will be at this position
		vec2 GetPosition() const { return m_position; }
		void SetPosition(const vec2& position) { m_position = position; }

		// the bounds of a tile relative to its location
		// controls the size of a tile
		// defaulted to {min = -0.5, max = 0.5f} where {width = 1.0f, height = 1.0f}
		rect GetRelativeTilebounds() const { return m_relativeTilebounds; }
		void SetRelativeTilebounds(const rect& relativeTilebounds) { m_relativeTilebounds = relativeTilebounds; }

		// the size of one tile on the given texture
		vec2 GetSpritesize() const { return m_spritesize; }
		void SetSpritesize(const vec2& spritesize) { m_spritesize = spritesize; }

		// the size of the tilemap 
		// resizing will remove or add to the ends
		uvec2 GetMapsize() const { return m_mapsize; }
		void SetMapsize(const uvec2& mapsize);

		// returns the tile at the given position
		Tile GetTile(const uint32 x, const uint32 y);
		Tile GetTile(const uvec2& pos);

		// returns the bounds of the given tile in its world position
		rect GetTileBounds(const uint32 x, const uint32 y);
		rect GetTileBounds(const uvec2& pos);

		// draws all tiles in the tilemap using a spritebatch
		void Draw(SpriteBatch& batch);

	private:

		vec2 m_position;
		vector<rect> m_spritebounds;
		vec2 m_spritesize;
		rect m_relativeTilebounds;
		vector<Tile> m_tiles;
		uvec2 m_mapsize;

	};

}

#endif // !ALC_2D_TILEMAP_HPP