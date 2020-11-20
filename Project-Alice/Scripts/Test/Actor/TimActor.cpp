#include "TimActor.hpp"

RTTR_REGISTRATION{
	registration::class_<TimActor>("TimActor")
	.public_object_constructor
	.property("actorName",&TimActor::actorName);

}



TimActor::TimActor():CActor("tim")
{	
	
}

TimActor::~TimActor() {

}

void TimActor::Start()
{
	sprite = AddComponent<SpriteRenderer>();
	sprite->LoadTexture("Cant Walk");
	sprite->SetTilingSize(vec2(16.0f));
	sprite->SetPivot(vec2(8.0f));
	sprite->SetLayer(2);
}

void TimActor::Update()
{

}
