#include "TestPhase.hpp"
#include "Engine/Game.hpp"


TestPhase::TestPhase():timer(0),timerEnd(0),result(BTAResult::Error) {
	
}

TestPhase::~TestPhase()
{

}

void TestPhase::StartPhase()
{
	//set phase varaiables to the enemy 
	SetPhaseName("Move From Middle");
	DEBUG_LOG("phase : " + GetPhaseName() + " has Started");
	SetPhaseDestination(glm::vec2(0.0f, 0.0f));

	timerEnd = 1;
}

void TestPhase::UpdatePhase()
{
	SetPosition(GetEnemy()->GetPosition());
	SetCurrentHealth(GetEnemy()->GetCurrentHealth());
	SetMaxHealth(GetEnemy()->GetMaxHealth());

	//SetAcceleration(GetEnemy()->GetMaxAcceleration());
	//SetMaxSpeed(GetEnemy()->GetMaxSpeed());
	vec2 velocity = GetEnemy()->GetVelocity();

	if (timer < (timerEnd * 40)) {
		//flee from the middle of window 
		result = bta::FleeFrom(&velocity, GetEnemyPosition(), GetPhaseDestination(), GetEnemyAcceleration(), GetEnemyMaxSpeed());

		if (timer >= (timerEnd * 39)) {
			//the last time if block is checked set destiation to enemyposition scaled by 2 for the move to function to work 
			SetPhaseDestination(GetEnemy()->GetPosition() * 2.0f);
		}
	}

	else {
		// call move too on new destination when flee timer is up to slow down when arriving at the destination
		result = bta::MoveTo(&velocity, GetEnemyPosition(), GetPhaseDestination(), GetEnemyAcceleration(), GetEnemyMaxSpeed());
	}

	GetEnemy()->SetVelocity(velocity);

	timer++;
}

bool TestPhase::IsComplete()
{
	if (result == BTAResult::Arrived) {
		DEBUG_LOG(GetPhaseName() + " phase is Complete ");
		timer = 0;
		result = BTAResult::Success;
		return true;
	} 

	return false;
}