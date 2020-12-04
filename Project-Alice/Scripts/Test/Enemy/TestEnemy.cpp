#include "TestEnemy.hpp"

RTTR_REGISTRATION{
	registration::class_<TestEnemy>("TestEnemy")
		.public_object_constructor;
//.property("destination",&TestEnemy::destination);
}


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
	moveFromMiddle = rttr_cast<TestPhase*>(AddPhase(moveFromMiddle->get_type())); 
	moveToMiddle = rttr_cast<TestPhase1*>(AddPhase(moveToMiddle->get_type()));	

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