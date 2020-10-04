#include "TestEnemy.hpp"

RTTR_REGISTRATION{
	registration::class_<TestEnemy>("TestEnemy")
		.public_object_constructor;
		//.property("destination",&TestEnemy::destination);
}


TestEnemy::TestEnemy() : sprite(nullptr) {}

TestEnemy::~TestEnemy()
{
	OnDestroy();
}


void TestEnemy::Start()
{
	sprite = AddComponent<Sprite>();
	sprite->LoadTexture("Flashing Grey Orb");
	sprite->SetTilingSize(vec2(16.0f));
	sprite->SetPivot(vec2(8.0f));
	sprite->SetLayer(2);
	srand(time(NULL));
	SetMaxSpeed(1.0f);
	destination = glm::vec2(25, -25);
	
}

void TestEnemy::Update()
{
	SeekAndArrive(destination, isMoving);
}

void TestEnemy::LateUpdate()
{
	
}

void TestEnemy::OnDestroy()
{
}