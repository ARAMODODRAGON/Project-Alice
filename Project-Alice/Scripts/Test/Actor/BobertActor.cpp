
#include "BobertActor.hpp"

RTTR_REGISTRATION{
	registration::class_<BobertActor>("BobertActor")
	.public_object_constructor
	.property("actorName",&BobertActor::actorName);

}

BobertActor::BobertActor():CActor("bobert")
{
}

BobertActor::~BobertActor()
{
}

void BobertActor::Start()
{
	sprite = AddComponent<SpriteRenderer>();
	sprite->LoadTexture("Flashing Grey Orb");
	sprite->SetTilingSize(vec2(16.0f));
	sprite->SetPivot(vec2(8.0f));
	sprite->SetLayer(2);
}

void BobertActor::Update()
{
}
