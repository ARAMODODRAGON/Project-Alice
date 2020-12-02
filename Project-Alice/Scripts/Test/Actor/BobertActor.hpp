#ifndef BOBERT_ACTOR_HPP
#define BOBERT_ACTOR_HPP

#include <Engine/Cutscene/CActor.hpp>

class BobertActor : public CActor
{
	std::string actorName;
	SpriteRenderer* sprite;
public:
	BobertActor();
	~BobertActor();

	void Start() override;
	void Update()override;

	RTTR_ENABLE(CActor) RTTR_REGISTRATION_FRIEND
};
#endif // !BOBERT_ACTOR_HPP

