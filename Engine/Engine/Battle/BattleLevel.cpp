#include "BattleLevel.hpp"

RTTR_REGISTRATION {
	registration::class_<BattleLevel>("BattleLevel")
		.public_object_constructor;
}

void BattleLevel::Init() { }

void BattleLevel::Update() { }

void BattleLevel::LateUpdate() { }

void BattleLevel::Exit() { }
