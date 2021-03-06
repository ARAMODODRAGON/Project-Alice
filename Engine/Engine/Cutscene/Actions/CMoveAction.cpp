#include "CMoveAction.hpp"
#include "../CutsceneManager.hpp"

CMoveAction::CMoveAction() :actor(nullptr), result(BTAResult::Error)
{
}

CMoveAction::~CMoveAction()
{
	actor = nullptr;
	delete actor;
}

void CMoveAction::Start()
{
	actor = CutsceneManager::GetActiveActor(actorName);
	actor->MoveAction(position);
}

bool CMoveAction::Update()
{
	if (actor->isRuning() && isRunning()) {
		glm::vec2 actorVelocity = actor->GetVelocity();
		result = bta::MoveTo(&actorVelocity, actor->GetPosition(), actor->GetDestination(), 10.0f, 10.0f);

		actor->SetVelocity(actorVelocity);

		actor->ActorDirection(actorVelocity);
	}

	else {
		return false;
	}
}

bool CMoveAction::isRunning()
{
	if (result == BTAResult::Arrived) {
		actor->SetCurrentAction(CurrentAction::NONE);
		return false;
	}
	else {
		return true;
	}
}
