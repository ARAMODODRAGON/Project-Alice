#ifndef C_ACTOR_HPP
#define C_ACTOR_HPP

#include "../Common.hpp"
#include "CutsceneManager.hpp"


enum class FacingDirection : unsigned char {
	DEFAULT = 'D',
	NORTH   = 'N',
	EAST    = 'E',
	SOUTH   = 'S',
	WEST    = 'W'
};

enum class CurrentAction : unsigned int {
	NONE     = 0,
	MOVING   = 1,
	SPEAKING = 2
};

class CActor : public Object
{

	std::string actorName;
	bool isControlled;
	FacingDirection facingDir; //direction the Actor is facing 
	CurrentAction curntAction;
	glm::vec2 destination;

protected:
public:
	CActor(std::string actorName_);
	virtual ~CActor() = 0;

	std::string GetActorName() { return actorName; }
	bool GetIsControlled() { return isControlled; }
	FacingDirection GetFacingDirection() { return facingDir; }
	CurrentAction GetCurrentAction() { return curntAction; }
	unsigned char GetFacingDirectionChar() { return (unsigned char)facingDir; }
	glm::vec2 GetDestination() { return destination; }

	void SetActorName(std::string actorName_) { actorName = actorName_; }
	void SetIsControlled(bool isControlled_) { isControlled = isControlled_; }
	void SetCurrentAction(CurrentAction curntAction_) { curntAction = curntAction_; }

	
	void Move(glm::vec2 velocity_);
	void MoveAction(glm::vec2 position_);
	FacingDirection ActorDirection(glm::vec2 actorVel_);
	bool isRuning();
	

	RTTR_ENABLE(Object) RTTR_REGISTRATION_FRIEND
};
#endif // !C_ACTOR_HPP

