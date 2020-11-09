#ifndef C_MOVE_ACTION_HPP
#define C_MOVE_ACTIOM_HPP

#include "../CAction.hpp"

class CMoveAction : CAction
{

	std::string name;
	glm::vec2 position;

	CActor* actor;

public :
	CMoveAction();
	~CMoveAction();

	void Start()override;
	bool Update()override;

	//getters and setters 
	std::string GetName() { return name; };
	glm::vec2 GetPosition() { return position; }

	void SetName(std::string name_) { name = name_; }
	void SetPosition(glm::vec2 position_) { position = position_; }

};
#endif

											