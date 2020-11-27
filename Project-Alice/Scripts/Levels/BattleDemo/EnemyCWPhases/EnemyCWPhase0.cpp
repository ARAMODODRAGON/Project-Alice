#include "EnemyCWPhase0.hpp"

EnemyCWPhase0::EnemyCWPhase0() : shc(nullptr) {
	shc = GetEnemy()->AddComponent<ShootComponent>();
	shc->Allocate(type::get<void>(), 20);
}

EnemyCWPhase0::~EnemyCWPhase0() { }

void EnemyCWPhase0::StartPhase() { 

}

void EnemyCWPhase0::UpdatePhase() { }

bool EnemyCWPhase0::IsComplete() {
	return false;
}
