#ifndef C_MOVE_ACTION_HPP
#define C_MOVE_ACTION_HPP

#include "../CAction.hpp"

class CMoveAction : public CAction
{

	std::string name;
	std::string actorName;
	glm::vec2 position;
	BTAResult result;
	CActor* actor;

public :
	CMoveAction();
	~CMoveAction();

	void Start()override;
	bool Update()override;
	bool isRunning()override;

	//getters and setters 
	std::string GetName() { return name; };	 
	std::string GetActorName() { return actorName; };	 
	glm::vec2 GetPosition() { return position; }

	void SetName(std::string name_) { name = name_; }
	void SetActorName(std::string actorName_) { actorName = actorName_; }
	void SetPosition(glm::vec2 position_) { position = position_; }

	RTTR_ENABLE(CAction)RTTR_REGISTRATION_FRIEND
};
#endif

											