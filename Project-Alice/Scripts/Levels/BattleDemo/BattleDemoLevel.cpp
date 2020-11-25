#include "BattleDemoLevel.hpp"
#include <Engine\Battle\BattleManager.hpp>

RTTR_REGISTRATION {
	registration::class_<BattleDemoLevel>("BattleDemoLevel")
		.public_object_constructor;
}

BattleDemoLevel::BattleDemoLevel() { }

BattleDemoLevel::~BattleDemoLevel() { }

void BattleDemoLevel::Init() {
	
	// setup camera
	camera = ObjectFactory::Make<BattleDemoCamera>();

	// setup the bounds of the level
	Rect levelBounds;
	constexpr float halfHeight = (288.0f / 2.0f) - 20.0f;
	levelBounds.max = vec2(halfHeight * 0.88, halfHeight);
	levelBounds.min = -levelBounds.max;
	BattleManager::SetBattleArea(levelBounds);

}

void BattleDemoLevel::Update() {

}

void BattleDemoLevel::LateUpdate() {

}

void BattleDemoLevel::Exit() {

}
