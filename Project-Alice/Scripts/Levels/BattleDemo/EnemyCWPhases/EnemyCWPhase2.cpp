#include "EnemyCWPhase2.hpp"
#include <Engine\Battle\BTPlayer.hpp>

RTTR_REGISTRATION {
	registration::class_<EnemyCWPhase2>("EnemyCWPhase2")
	.public_object_constructor;
}

EnemyCWPhase2::EnemyCWPhase2() { }

EnemyCWPhase2::~EnemyCWPhase2() { }

void EnemyCWPhase2::Init() { }

void EnemyCWPhase2::StartPhase() {
	DEBUG_LOG("State 2 started");
	GetEnemy()->SetMaxHealth(100.0f);
	GetEnemy()->SetCurrentHealth(100.0f);
	BattleManager::SetTimer(20.0f);
}

void EnemyCWPhase2::UpdatePhase() { }

bool EnemyCWPhase2::IsComplete() {
	if (GetEnemy()->GetCurrentHealth() <= 0.0f) {
		DEBUG_LOG("Enemy health was 0");
		return true;
	} else if (BattleManager::GetTimer() <= 0.0f) {
		DEBUG_LOG("Timer was 0");
		return true;
	} else if (BattleManager::GetPlayer(0)->GetHitpoints() == 0)
		return true;
	return false;
}
