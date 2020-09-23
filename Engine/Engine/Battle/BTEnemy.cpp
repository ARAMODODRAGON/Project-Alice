#include "BTEnemy.hpp"

RTTR_REGISTRATION{
	registration::class_<BTEnemy>("BTEnemy")
		.property("movmentSpeed",&BTEnemy::movementSpeed);
}

BTEnemy::BTEnemy() : movementSpeed(NULL) { BattleManager::AddEnemy(this); }

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