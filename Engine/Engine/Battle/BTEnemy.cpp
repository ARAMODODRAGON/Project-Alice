#include "BTEnemy.hpp"
#include "../Game.hpp"

RTTR_REGISTRATION{
	registration::class_<BTEnemy>("BTEnemy")
		.property("movmentSpeed",&BTEnemy::maxSpeed)
		.property("destination",&BTEnemy::destination)
		.property("isMoving",&BTEnemy::isMoving)
		.property("maxAcceleration",&BTEnemy::maxAcceleration);
}

BTEnemy::BTEnemy() : maxSpeed(NULL),destination(vec2()),isMoving(false),maxAcceleration(NULL) { BattleManager::AddEnemy(this); }

BTEnemy::~BTEnemy() { BattleManager::RemoveEnemy(this); }

void BTEnemy::Start()
{
}

void BTEnemy::Update()
{
}

void BTEnemy::LateUpdate()
{
}

void BTEnemy::OnDestroy()
{
	

}

void BTEnemy::SeekAndArrive(glm::vec2 destination_, bool isMoving_)
{
	SetVelocity(bta::MoveTo(GetPosition(), destination_, maxAcceleration, maxSpeed));	
}



