#include "TestEnemy.hpp"

RTTR_REGISTRATION{
	registration::class_<TestEnemy>("TestEnemy")
		.public_object_constructor;
//.property("destination",&TestEnemy::destination);
}


TestEnemy::TestEnemy() : sprite(nullptr), moveFromMiddle() {}

TestEnemy::~TestEnemy()
{
	OnDestroy();
}


void TestEnemy::Start()
{
	sprite = AddComponent<SpriteRenderer>();
	sprite->LoadTexture("Flashing Grey Orb");
	sprite->SetTilingSize(vec2(16.0f));
	sprite->SetPivot(vec2(8.0f));
	sprite->SetLayer(2);
	//srand(time(NULL));
	moveFromMiddle = new TestPhase;
	SetMaxSpeed(50.0f);
	SetMaxAcceleration(50.0f);
	//SetDestination(glm::vec2(25, -25));
	moveFromMiddle->SetEnemy(this);
   AddPhase(moveFromMiddle->get_type());
	

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
	//delete sprite;
	//sprite = nullptr;

	delete moveFromMiddle;
	moveFromMiddle = nullptr;
}