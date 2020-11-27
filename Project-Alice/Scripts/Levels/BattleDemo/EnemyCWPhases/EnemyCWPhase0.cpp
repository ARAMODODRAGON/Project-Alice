#include "EnemyCWPhase0.hpp"

RTTR_REGISTRATION {
	registration::class_<EnemyCWPhase0>("EnemyCWPhase0")
		.public_object_constructor;
}

EnemyCWPhase0::EnemyCWPhase0() : shc(nullptr) { }

EnemyCWPhase0::~EnemyCWPhase0() { }

void EnemyCWPhase0::Init() {
	if (shc = GetEnemy()->AddComponent<ShootComponent>()) {
		shc->Allocate(type::get<void>(), 20);
	}
}

void EnemyCWPhase0::StartPhase() {

}

void EnemyCWPhase0::UpdatePhase() { }

bool EnemyCWPhase0::IsComplete() {
	return false;
}
