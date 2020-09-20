#include "TestEnemy.hpp"

RTTR_REGISTRATION{
	registration::class_<TestEnemy>("TestEnemy")
		.public_object_constructor;
}


TestEnemy::TestEnemy() : sprite(nullptr)
{

}

TestEnemy::~TestEnemy()
{
	OnDestroy();
}

void TestEnemy::SeekAndArrive(vec2 destination_, vec2 enemyPosition_)
{

}

void TestEnemy::Start()
{
	sprite = AddComponent<Sprite>();
	//sprite->SetIsActive(true);
	sprite->SetPivot(vec2(4.5f, 14.5f)); // center pivot
	//sprite->SetColor(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	//sprite->LoadTexture("Test 0");
	sprite->LoadTexture("Test Character");
	sprite->SetTilingSize(vec2(17.0f, 29.0f));
	sprite->SetTilingOffset(vec2(4.0f, 7.0f));
	sprite->SetTilingMargin(vec2(7.0f, 7.0f));
	sprite->SetLayer(-2);
	
}

void TestEnemy::Update()
{
}

void TestEnemy::LateUpdate()
{
}

void TestEnemy::OnDestroy()
{

}