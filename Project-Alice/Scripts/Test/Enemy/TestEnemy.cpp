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
	//srand(time(NULL));
	SetMaxSpeed(100.0f);
	SetMaxAcceleration(100.0f);
	SetDestination (glm::vec2(25, -25));
	
}

void TestEnemy::Update()
{
	vec2 velocity = GetVelocity();

	BTAResult result = bta::MoveTo(&velocity, GetPosition(), GetDestination(), GetMaxSpeed(), 1.0f);
	if (result == BTAResult::Arrived) {
		
		vec2 newdest;
		newdest.x = (rand() % 400) * 0.1f - 5.0f;
		newdest.y = (rand() % 400) * 0.1f - 5.0f;
		SetDestination(newdest);

		DEBUG_LOG("Changed destination to: " + VTOS(newdest));
	}

	SetVelocity(velocity);

}

void TestEnemy::LateUpdate()
{
	
}

void TestEnemy::OnDestroy()
{
}