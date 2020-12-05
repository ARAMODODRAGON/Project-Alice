#include "BattleDemoLevel.hpp"
#include <Engine\Battle\BattleManager.hpp>

BattleDemoLevel::BattleDemoLevel() { }

BattleDemoLevel::~BattleDemoLevel() { }

void BattleDemoLevel::Init() {
	
	// setup camera
	camera = ObjectFactory::Make<BattleDemoCamera>();
	// camera size = (512, 288)

	// setup the bounds of the level
	Rect levelBounds;
	constexpr float halfHeight = (288.0f / 2.0f) - 15.0f;
	constexpr float ratio = 0.88f;
	levelBounds.top = halfHeight;
	levelBounds.bottom = -halfHeight;
	levelBounds.right = halfHeight * ratio;
	levelBounds.left = -levelBounds.right;
	BattleManager::SetBattleArea(levelBounds);

	// add player & enemy
	player = ObjectFactory::Make<BDPlayer>();
	enemy = ObjectFactory::Make<BDEnemyCW>();

}

void BattleDemoLevel::Update() {

}

void BattleDemoLevel::LateUpdate() {

}

void BattleDemoLevel::Exit() {

}