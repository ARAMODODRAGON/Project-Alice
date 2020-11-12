#ifndef TIM_ACTOR_HPP
#define TIM_ACTOR_HPP

#include <Engine/Cutscene/CActor.hpp>

class TimActor : public CActor
{
	std::string actorName;
	SpriteRenderer* sprite;
public:			  
	TimActor();
	//TimActor(std::string actorName_) : CActor("tim") {}
	~TimActor();

	void Start() override;
	void Update()override;

	RTTR_ENABLE(CActor) RTTR_REGISTRATION_FRIEND
};
#endif // !TIM_ACTOR_HPP
