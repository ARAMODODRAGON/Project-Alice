#include "MoveStates.hpp"
#include <ALC/Physics/Transform2D.hpp>
#include "../Bullets.hpp"

#include "../BattleManager.hpp"
#include "../Enemies/Enemy.hpp"


void MoveStates::PerformMoveState(Enemy* enemy,States moveState, ALC::uint8 *_curntState, ALC::Timestep ts, 
	                              float moveTime, ALC::uint8 nextState,ALC::vec2 dest)
{
	auto lvlBounds = BattleManager::GetLevelBounds();
	
	auto [trans, eBody] = enemy->GetEntity().GetComponent< ALC::Transform2D, CharacterBody>();
	auto arivd = BTA::Result::Arrived;

	//if the previous state is the same as the current state given return out of the function 
	if (prevState == moveState) {
		return;
	} 
	// if a move time is not NULL start counter 
	if (moveTime > 0.0f) {
		timer += ts.Get();
	}

	float right = (lvlBounds.right - 60);
	float left = (lvlBounds.left + 60);
	float up = (lvlBounds.top - 61);
	float down = (lvlBounds.top / 4);
	isComplete = false;
	auto result = BTA::Result::Error;
	e_acceleration = enemy->GetAccel();
	e_speed = enemy->GetSpeed();
								
	if (e_speed <= 0.0f || e_acceleration <= 0.0f) {
		e_speed = e_acceleration = 100.0f;
	}

	switch (static_cast<ALC::uint8>(moveState)) {

	case 1:	// move
		//no destination given and move state is move just go to center
		if (dest == ALC::vec2(0, 0) && moveState == States::Move) {
			dest = ALC::vec2( (lvlBounds.left + lvlBounds.right),(lvlBounds.top / 2));
		}

		result = BTA::MoveTo(&eBody.velocity, trans.position, dest, e_acceleration, e_speed, ts);
		//Once the destination is reached
		if (result == arivd) {
			prevState = moveState;	// assign prev move state to current move state 
			if (nextState != 0) { *_curntState = nextState; }	//assign a next state to curnt state if one is provided
			isComplete = true;	// set complete to true so the AI can take the necessary steps once move is done 
		}

		//same thing as above but with the timer 
		//not really tested but should work ..... maybe ....
		else if (timer > moveTime) {
			prevState = moveState;
			if (nextState != 0) { *_curntState = nextState; }
			isComplete = true;
		}

		break;

	case 2:	 //right

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

	case 3:	//left

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

	case 4:	//up

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

	case 5:	//Down

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

	case 6:	//Up Left

		//these checks are in all the 2 dir movments to check if the AI is already at one of the desired destinations 
		//for example if up left is called but the AI is alread all the way left just move up 
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

	case 7://Down Left

	
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

	case 8:	 //Up right

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

	case 9:	 //Down right

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
