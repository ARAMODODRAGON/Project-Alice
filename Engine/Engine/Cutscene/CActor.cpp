#include "CActor.hpp"

CActor::CActor(std::string actorName_):facingDir(FacingDirection::DEFAULT),curntAction(CurrentAction::NONE)
{
	actorName = actorName_;
	CutsceneManager::AddActor(actorName_, this);
}

CActor::~CActor()
{
	CutsceneManager::RemoveActor(actorName,this);
}

FacingDirection CActor::ActorDirection(glm::vec2 actorVel_)
{

	//absolute value of the actor velocity 
	float absVelX = glm::abs(actorVel_.x);
	float absVelY = glm::abs(actorVel_.y);

	//check direction actor is going it 

	if (absVelX > absVelY) {

		if (actorVel_.x < 0) {

			return FacingDirection::WEST;

		}
		else { return FacingDirection::EAST; }
	}

	else {

		if (actorVel_.y < 0) {

			return FacingDirection::SOUTH;

		}
		else { return FacingDirection::NORTH; }
	}

	return FacingDirection::DEFAULT;

}

void CActor::Move(glm::vec2 velocity_)
{
	if (!isControlled) {
		SetVelocity(velocity_);
		facingDir = ActorDirection(GetVelocity());
	}
	else { DEBUG_LOG("Actor is already being controlled"); }
}

void CActor::MoveAction(glm::vec2 position_)
{
	if (curntAction == CurrentAction::NONE) {
		curntAction = CurrentAction::MOVING;
	}
	destination = position_;
}

bool CActor::isRuning()
{
	switch ((int)curntAction) {
		case 0:
			return false;
		break;

		case 1:
			return true;
		break;

		case 2:
			return true;
		break;
	}

}

