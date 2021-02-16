#include "MoveStates.hpp"
#include <ALC/Physics/Transform2D.hpp>
#include <ALC/Bullets/BulletPhysics.hpp>

#include "../BattleManager.hpp"
#include "../Enemies/Enemy.hpp"

//TODO:: add a way so so the state knows its done before moving on to the next state either in this class or the enemy with a list or vector
void MoveStates::PerformMoveState(Enemy* enemy,States moveState, ALC::uint8 *_curntState, ALC::Timestep ts, 
	                              float moveTime, ALC::uint8 nextState,ALC::vec2 dest)
{
	auto lvlBounds = BattleManager::GetLevelBounds();
	
	auto [trans, eBody] = enemy->GetEntity().GetComponent< ALC::Transform2D, ALC::CharacterBody>();
	auto arivd = BTA::Result::Arrived;
	if (prevState == moveState) {
		return;
	} 
	
	if (dest == ALC::vec2(0,0)) {
		dest = ALC::vec2((lvlBounds.top / 2), (lvlBounds.left / 2));
	}

	if (moveTime > 0.0f) {
		timer += ts.Get();
	}

	float right = (lvlBounds.right - 20);
	float left = (lvlBounds.left + 20);
	float up = (lvlBounds.top - 21);
	float down = (lvlBounds.top / 4);
	isComplete = false;
	auto result = BTA::Result::Error;
	e_acceleration = enemy->GetAccel();
	e_speed = enemy->GetSpeed();
								
	if (e_speed <= 0.0f || e_acceleration <= 0.0f) {
		e_speed = e_acceleration = 100.0f;
	}

	switch (static_cast<ALC::uint8>(moveState)) {

	case 1:

		result = BTA::MoveTo(&eBody.velocity, trans.position, dest, e_acceleration, e_speed, ts);

		if (result == arivd) {
			prevState = moveState;
			if (nextState != 0) { *_curntState = nextState; }
			isComplete = true;
		}

		else if (timer > moveTime) {
			prevState = moveState;
			if (nextState != 0) { *_curntState = nextState; }
			isComplete = true;
		}

		break;

	case 2:

		dest = ALC::vec2(right, trans.position.y);

		result = BTA::MoveTo(&eBody.velocity, trans.position, dest, e_acceleration, e_speed, ts);

		if (result == arivd) {
			prevState = moveState;
			if (nextState != 0) { *_curntState = nextState; }
			isComplete = true;
		}

		else if (timer > moveTime) {
			prevState = moveState;
			if (nextState != 0) { *_curntState = nextState; }
			isComplete = true;
		}

		break;

	case 3:

		dest = ALC::vec2(left, trans.position.y);

		 result = BTA::MoveTo(&eBody.velocity, trans.position, dest, e_acceleration, e_speed, ts);

		if (result == arivd) {
			prevState = moveState;
			if (nextState != 0) { *_curntState = nextState; }
			isComplete = true;
		}

		else if (timer > moveTime) {
			prevState = moveState;
			if (nextState != 0) { *_curntState = nextState; }
			isComplete = true;
		}

		break;

	case 4:

		dest = ALC::vec2(trans.position.x, up);

		 result = BTA::MoveTo(&eBody.velocity, trans.position, dest, e_acceleration, e_speed, ts);

		if (result == arivd) {
			prevState = moveState;
			if (nextState != 0) { *_curntState = nextState; }
			isComplete = true;
		}

		else if (timer > moveTime) {
			prevState = moveState;
			if (nextState != 0) { *_curntState = nextState; }
			isComplete = true;
		}

		break;

	case 5:

		dest = ALC::vec2(trans.position.x, down);
		 result = BTA::MoveTo(&eBody.velocity, trans.position, dest, e_acceleration, e_speed, ts);

		if (result == arivd) {
			prevState = moveState;
			if (nextState != 0) { *_curntState = nextState; }
			isComplete = true;
		}

		else if (timer > moveTime) {
			prevState = moveState;
			if (nextState != 0) { *_curntState = nextState; }
			isComplete = true;
		}

		break;

	case 6:

		if (trans.position.x <= left) {
			dest = ALC::vec2(trans.position.x, up);
		}
		else if (trans.position.y >= up) {
			dest = ALC::vec2(left, trans.position.y);
		}
		else {
			dest = ALC::vec2(left, up);
		}

		 result = BTA::MoveTo(&eBody.velocity, trans.position, dest, e_acceleration, e_speed, ts);

		if (result == arivd) {
			prevState = moveState;
			if (nextState != 0) { *_curntState = nextState; }
			isComplete = true;
		}

		else if (timer > moveTime) {
			prevState = moveState;
			if (nextState != 0) { *_curntState = nextState; }
			isComplete = true;
		}

		break;

	case 7:

		if (trans.position.x <= left) {
			dest = ALC::vec2(trans.position.x, down);
		}
		else if (trans.position.y <= down) {
			dest = ALC::vec2(left, trans.position.y);
		}
		else {
			dest = ALC::vec2(left, down);
		}

		 result = BTA::MoveTo(&eBody.velocity, trans.position, dest, e_acceleration, e_speed, ts);

		if (result == arivd) {
			prevState = moveState;
			if (nextState != 0) { *_curntState = nextState; }
			isComplete = true;
		}

		else if (timer > moveTime) {
			prevState = moveState;
			if (nextState != 0) { *_curntState = nextState; }
			isComplete = true;
		}

		break;

	case 8:

		if (trans.position.x >= right) {
			dest = ALC::vec2(trans.position.x, up);
		}
		else if (trans.position.y >= up) {
			dest = ALC::vec2(right, trans.position.y);
		}
		else {
			dest = ALC::vec2(right, up);
		}

		 result = BTA::MoveTo(&eBody.velocity, trans.position, dest, e_acceleration, e_speed, ts);

		if (result == arivd) {
			prevState = moveState;
			if (nextState != 0) { *_curntState = nextState; }
			isComplete = true;
		}

		else if (timer > moveTime) {
			prevState = moveState;
			if (nextState != 0) { *_curntState = nextState; }
			isComplete = true;
		}

		break;

	case 9:

		if (trans.position.x >= right) {
			dest = ALC::vec2(trans.position.x, down);
		}
		else if (trans.position.y <= down) {
			dest = ALC::vec2(right, trans.position.y);
		}
		else {
			dest = ALC::vec2(right, down);
		}

		 result = BTA::MoveTo(&eBody.velocity, trans.position, dest, e_acceleration, e_speed, ts);

		if (result == arivd) {
			prevState = moveState;
			if (nextState != 0) { *_curntState = nextState; }
			isComplete = true;
		}

		else if (timer > moveTime) {
			prevState = moveState;
			if (nextState != 0) { *_curntState = nextState; }
			isComplete = true;
		}

		break;

	}

}
