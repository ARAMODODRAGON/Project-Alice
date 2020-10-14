#include "TestPhase.hpp"

RTTR_REGISTRATION{
	registration::class_<TestPhase>("TestPhase")
	.public_object_constructor;
}

TestPhase::TestPhase() {
}

TestPhase::~TestPhase()
{

}

void TestPhase::StartPhase()
{
	//set phase varaiables to the enemy 
	SetPhaseName("Move From Middle");
	SetPosition(GetEnemy()->GetPosition());
	//SetPhaseDestination(glm::vec2(10.0f, 10.0f));
	SetCurrentHealth(GetEnemy()->GetCurrentHealth());
	SetAcceleration(GetEnemy()->GetMaxAcceleration());
	SetMaxSpeed(GetEnemy()->GetMaxSpeed());
	SetMaxHealth(GetEnemy()->GetMaxHealth());

}

void TestPhase::UpdatePhase()
{
	vec2 velocity = GetEnemy()->GetVelocity();

	BTAResult result = bta::MoveTo(&velocity, GetEnemyPosition(), GetPhaseDestination(), GetEnemyMaxSpeed(), 1.0f);
	if (result == BTAResult::Arrived) {

		vec2 newdest;
		newdest.x = (rand() % 400) * 0.1f - 5.0f;
		newdest.y = (rand() % 400) * 0.1f - 5.0f;
		SetPhaseDestination(newdest);

		DEBUG_LOG("Changed destination to: " + VTOS(newdest));
	}

	GetEnemy()->SetVelocity(velocity);
	DEBUG_LOG("Enemy Current Pos : " + VTOS(GetEnemyPosition()));
}

bool TestPhase::isComplete()
{
	

	return false;
}