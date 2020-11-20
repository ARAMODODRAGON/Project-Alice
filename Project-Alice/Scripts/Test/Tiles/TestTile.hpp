#ifndef TEST_TILE_HPP
#define TEST_TILE_HPP

#include <Engine/Battle/TileMap/Tile.hpp>


class TestTile : public Tile
{
	SpriteRenderer* sprite;
public:
	TestTile();
	~TestTile();

	void Start() override;
	
};
#endif
