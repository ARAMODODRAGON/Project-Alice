#include "TestPhase.hpp"
#include "Engine/Game.hpp"


RTTR_REGISTRATION{
	registration::class_<TestPhase>("TestPhase")
	.public_object_constructor
	.property("timer",&TestPhase::timer)
	.property("timerEnd",&TestPhase::timerEnd)
	.property("result",&TestPhase::result);
}

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

	SetAcceleration(GetEnemy()->GetMaxAcceleration());
	SetMaxSpeed(GetEnemy()->GetMaxSpeed());

	if (timer < (timerEnd * 40)) {
		//flee from the middle of window 
		DEBUG_LOG("Fleeing");
		vec2 velocity = GetEnemy()->GetVelocity();
		result = bta::FleeFrom(&velocity, GetEnemyPosition(), GetPhaseDestination(), GetEnemyAcceleration(), GetEnemyMaxSpeed());
		GetEnemy()->SetVelocity(velocity);
		if (timer >= (timerEnd * 39)) {
			DEBUG_LOG("Changed Destination should only be called once ");
			SetPhaseDestination(GetEnemy()->GetPosition() * 2.0f);
		}
	}
	else {
		vec2 velocity = GetEnemy()->GetVelocity();
		result = bta::MoveTo(&velocity, GetEnemyPosition(), GetPhaseDestination(), GetEnemyAcceleration(), GetEnemyMaxSpeed());
		GetEnemy()->SetVelocity(velocity);
	}

	timer++;
}

bool TestPhase::isComplete()
{
	if (result == BTAResult::Arrived) {
		DEBUG_LOG(GetPhaseName() + " phase is Complete ");
		timer = 0;
		result = BTAResult::Success;
		return true;
	} 

	return false;
}