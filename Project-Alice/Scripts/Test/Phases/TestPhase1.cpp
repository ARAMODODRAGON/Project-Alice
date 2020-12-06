#include "TestPhase1.hpp"

TestPhase1::TestPhase1()
{
}

TestPhase1::~TestPhase1()
{
}

void TestPhase1::StartPhase()
{
	SetPhaseName("Move To Middle");	
	DEBUG_LOG("phase : " + GetPhaseName() + " has Started");
	SetPhaseDestination(glm::vec2(0.0f, 0.0f));

}

void TestPhase1::UpdatePhase()
{
	SetPosition(GetEnemy()->GetPosition());
	SetCurrentHealth(GetEnemy()->GetCurrentHealth());
	SetMaxHealth(GetEnemy()->GetMaxHealth());

	//SetAcceleration(GetEnemy()->GetMaxAcceleration());
	//SetMaxSpeed(GetEnemy()->GetMaxSpeed());



	//flee from the middle of window 
	vec2 velocity = GetEnemy()->GetVelocity();
	result = bta::MoveTo(&velocity, GetEnemyPosition(), GetPhaseDestination(), GetEnemyAcceleration(), GetEnemyMaxSpeed());
	GetEnemy()->SetVelocity(velocity);
}

bool TestPhase1::IsComplete()
{
	if (result == BTAResult::Arrived) {
		DEBUG_LOG(GetPhaseName() + " phase is Complete ");
		result = BTAResult::Moving;
		return true;
	}

	return false;
}
