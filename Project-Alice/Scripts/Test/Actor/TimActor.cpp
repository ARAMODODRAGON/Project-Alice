#include "TimActor.hpp"

RTTR_REGISTRATION{
	registration::class_<TimActor>("TimActor")
	.public_object_constructor
	.property("actorName",&TimActor::actorName);

}



TimActor::TimActor():CActor("tim")
{	
	sprite = AddComponent<SpriteRenderer>();
	sprite->LoadTexture("Flashing Grey Orb");
	sprite->SetTilingSize(vec2(16.0f));
	sprite->SetPivot(vec2(8.0f));
	sprite->SetLayer(2);
}

TimActor::~TimActor() {

}

void TimActor::Start()
{
	
}

void TimActor::Update()
{

}
