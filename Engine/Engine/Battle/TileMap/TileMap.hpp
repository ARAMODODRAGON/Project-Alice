#ifndef TILE_MAP_HPP
#define TILE_MAP_HPP
#include "../../Common.hpp"
#include "../../Rendering/SpriteRenderer.hpp"
#include<iostream>

#define ROW            7
#define COL            13

class Tile;

class TileMap
{
	PRIVATE_SINGLETON(TileMap);
	std::vector<Tile*> tilesVector;
	
	int32 tileMap[ROW][COL];
	 
public:
	TileMap();
	~TileMap();

	static void LoadMap(int32 arr[ROW][COL]);
	static void Init();

	static void AddTile(Tile* tile_);

	static std::vector<Tile*> GetTileVector() {
		auto& tilesVector_ = Get()->tilesVector;
		return tilesVector_;
	}

	static int32 GetTileElement(int32 row_, int32 col_) { 
		auto& tileMap_ = Get()->tileMap;
		return tileMap_[row_][col_]; 
	}

	static int32 SetTileElement(int32 row_, int32 col_, int32 value_) { 
		auto& tileMap_ = Get()->tileMap;
		tileMap_[row_][col_] = value_; 
	}

};
#endif;

