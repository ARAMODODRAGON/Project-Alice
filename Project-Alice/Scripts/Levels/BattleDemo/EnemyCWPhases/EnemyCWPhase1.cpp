#include "EnemyCWPhase1.hpp"

EnemyCWPhase1::EnemyCWPhase1() { }

EnemyCWPhase1::~EnemyCWPhase1() { }

void EnemyCWPhase1::Init() { }

void EnemyCWPhase1::StartPhase() {
	DEBUG_LOG("State 1 started");
	GetEnemy()->SetMaxHealth(100.0f);
	GetEnemy()->SetCurrentHealth(100.0f);
	BattleManager::SetTimer(20.0f);
}

void EnemyCWPhase1::UpdatePhase() { }

bool EnemyCWPhase1::IsComplete() {
	if (GetEnemy()->GetCurrentHealth() <= 0.0f) {
		DEBUG_LOG("Enemy health was 0");
		return true;
	} else if (BattleManager::GetTimer() <= 0.0f) {
		DEBUG_LOG("Timer was 0");
		return true;
	}
	return false;
}
