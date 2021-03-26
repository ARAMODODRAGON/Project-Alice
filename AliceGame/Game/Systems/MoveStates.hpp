#ifndef MOVE_STATES_HPP
#define MOVE_STATES_HPP
#include <ALC/Entities.hpp>
#include "../BTA.hpp"

class Enemy;

class MoveStates{
public:
	enum class States : ALC::uint8 {
		None,
		Move,
		Right,
		Left,
		Up,
		Down,
		UpLeft,
		DownLeft,
		UpRight,
		DownRight
	};

	MoveStates() : prevState(States::None), curntState(States::None), timer(NULL), e_acceleration(0.0f), e_speed(0.0f), isComplete(false) { }

	bool GetIsComplete() { return isComplete; }
	void SetIsComplete(bool isComplete_) { isComplete = isComplete_; }


	//Function that moves the AI to a desired destination either all the way to the bounds r over a certian amount of time 
	//takes the current state and assigns it a next state if one is given and returns it 
	//uses BTA moveTo function to move the AI
	void PerformMoveState(Enemy* enemy, States moveState, ALC::uint8 *_curntState, ALC::Timestep ts, 
		                  float moveTime = NULL, ALC::uint8 nextState = 0,ALC::vec2 dest = ALC::vec2(0,0));

private:

	float e_speed;
	float e_acceleration;
	States prevState;
	States curntState;
	float timer;
	bool isComplete;
};
#endif // !


