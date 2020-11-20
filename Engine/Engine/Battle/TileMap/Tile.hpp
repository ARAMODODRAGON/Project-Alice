#ifndef TILE_HPP
#define TILE_HPP
#include "../../Common.hpp"

class Tile : public Object
{
	bool isWalkable;
	int32 width;
	int32 height;


public:
	Tile();
	virtual ~Tile();


	bool GetIsWalkable() { return isWalkable; }
	vec2 GetHeightAndWidth() { return vec2(width, height); }  //x is width and y is height 


	void SetIsWalkable(bool isWalkable_) { isWalkable = isWalkable_; }
	void SetWidthAndHeight(int32 width_, int32 height_) { 
		width = width_;
		height = height_;
	}

	RTTR_ENABLE(Object) RTTR_REGISTRATION_FRIEND
};
#endif // !TILE_HPP