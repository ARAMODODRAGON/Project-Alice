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
	SetPhaseDestination(glm::vec2(0.0f, 0.0f));
	SetCurrentHealth(GetEnemy()->GetCurrentHealth());
	SetAcceleration(GetEnemy()->GetMaxAcceleration());
	SetMaxSpeed(GetEnemy()->GetMaxSpeed());
	SetMaxHealth(GetEnemy()->GetMaxHealth());

}

void TestPhase::UpdatePhase()
{

	//vec2 velocity = GetEnemy()->GetVelocity();

	//BTAResult result = bta::FleeFrom(&velocity, GetEnemyPosition(), GetPhaseDestination(), GetEnemyAcceleration(), GetEnemyMaxSpeed());
	//if (result == BTAResult::Arrived) {
	//	DEBUG_LOG("Enemy phase : " + GetPhaseName() + "is Complete ");

	//}

	//GetEnemy()->SetVelocity(velocity);
	//DEBUG_LOG("Updating Test Phase");
	DEBUG_LOG("Enemy Current Pos : " + VTOS(GetEnemyPosition()));
}

bool TestPhase::isComplete()
{
	return false;
}