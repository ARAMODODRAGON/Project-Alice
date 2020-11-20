#include "TileMap.hpp"


int32 levelTiles[ROW][COL] = {
		{0,  0, -1, -1, -1, -1,  0,  0,  0, -1, -1, -1, -1},
	    {-1, 0, -1, -1, -1, -1,  0, -1,  0, -1, -1, -1, -1},
		{-1, 0, -1, -1, -1, -1,  0, -1,  0, -1, -1, -1, -1},
		{0,  0,  0, -1,  0,  0,  0, -1,  0,  0,  0, -1,  0},
		{0, -1,  0, -1,  0, -1, -1, -1, -1, -1,  0, -1,  0},
		{0, -1,  0, -1,  0,  0,  0, -1, -1, -1,  0, -1,  0},
		{0, -1,  0,  0,  0, -1, -1, -1, -1, -1,  0,  0,  0},
};


TileMap::TileMap()
{

}

TileMap::~TileMap()
{
}

void TileMap::LoadMap(int32 arr[ROW][COL])
{
	auto& tileMap_ = Get()->tileMap;

	for (size_t i = 0; i < ROW; i++) {

		for (size_t j = 0; j < COL; ++j) {

			tileMap_[i][j] = levelTiles[i][j];


			/*int32 ranNum = rand() % 10 + 1;

			if (ranNum >= 4) {
				tiles_[i][j] = 'W';
			}
			else {
				tiles_[i][j] = 'N';
			} 
			
			std::cout << "tiles at " << "[" << i << "]" << "[" << j << "]" << " is " << tiles_[i][j] << std::endl; */
			
		}
	}
}

void TileMap::Init()
{
	LoadMap(levelTiles);
}

void TileMap::AddTile(Tile* tile_)
{
	auto& tilesVector_ = Get()->tilesVector;
	tilesVector_.push_back(tile_);
}






																																				  







