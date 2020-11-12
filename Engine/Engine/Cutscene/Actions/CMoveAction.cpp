#include "CMoveAction.hpp"

CMoveAction::CMoveAction() :actor(nullptr)
{
}

CMoveAction::~CMoveAction()
{
	actor = nullptr;
	delete actor;
}

void CMoveAction::Start()
{
	actor = CutsceneManager::GetActiveActor(name);
	actor->MoveAction(position);
}

bool CMoveAction::Update()
{
	if (!actor->isRuning()) {
		glm::vec2 actorVelocity = actor->GetVelocity();

		BTAResult result = bta::MoveTo(&actorVelocity, actor->GetPosition(), actor->GetDestination(), 10.0f, 10.0f);

		actor->SetVelocity(actorVelocity);

		actor->ActorDirection(actorVelocity);

		if (result == BTAResult::Arrived) {
			actor->SetCurrentAction(CurrentAction::NONE);
		}
	}

	else {
		return false;
	}
}
