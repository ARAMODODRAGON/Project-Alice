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

};
#endif // !BOBERT_ACTOR_HPP

