#include "TestEnemy.hpp"


TestEnemy::TestEnemy() : sprite(nullptr), moveFromMiddle(nullptr),moveToMiddle(nullptr) {}

TestEnemy::~TestEnemy()
{
	//OnDestroy();
}


void TestEnemy::Start()
{
	sprite = AddComponent<SpriteRenderer>();
	sprite->LoadTexture("Flashing Grey Orb");
	sprite->SetTilingSize(vec2(16.0f));
	sprite->SetPivot(vec2(8.0f));
	sprite->SetLayer(1);
	//srand(time(NULL));
	//moveFromMiddle = new TestPhase();
	//moveToMiddle = new TestPhase1();
	//SetMaxSpeed(50.0f);
	//SetMaxAcceleration(50.0f);
	moveFromMiddle = AddPhase<TestPhase>(); 
	moveToMiddle = AddPhase<TestPhase1>();	

}

void TestEnemy::Update()
{
	BTEnemy::Update();

}

void TestEnemy::LateUpdate()
{
	
}

void TestEnemy::OnDestroy()
{
}