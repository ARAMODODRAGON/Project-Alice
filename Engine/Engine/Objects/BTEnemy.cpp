#include "BTEnemy.hpp"

RTTR_REGISTRATION{
	registration::class_<BTEnemy>("BTEnemy")
		.property("movmentSpeed",&BTEnemy::movmentSpeed);
}

BTEnemy::BTEnemy() : movmentSpeed(0) { BattleManager::AddEnemy(this); }

BTEnemy::~BTEnemy() { OnDestroy(); }

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
	BattleManager::RemoveEnemy(this);

}