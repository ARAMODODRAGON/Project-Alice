#include "TestTile.hpp"

TestTile::TestTile()
{
	SetWidthAndHeight(16, 16);
}

TestTile::~TestTile()
{
}

void TestTile::Start()
{
	sprite = AddComponent<SpriteRenderer>();
	sprite->LoadTexture("Cant Walk");
	sprite->SetLayer(3);

	if (GetIsWalkable()) {
		sprite->SetIsActive(false);

	}
	else {
		sprite->SetIsActive(true);
	}

}
