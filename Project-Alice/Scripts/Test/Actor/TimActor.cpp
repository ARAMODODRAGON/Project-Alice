#include "TimActor.hpp"

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
