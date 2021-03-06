#include "BattleDemoEnemyCW.hpp"

BDEnemyCW::BDEnemyCW() : ph0(nullptr), spr(nullptr) {

}

BDEnemyCW::~BDEnemyCW() { }

void BDEnemyCW::Start() {
	// add components
	if (spr = AddComponent<SpriteRenderer>()) {
		spr->LoadTexture("64by64texture");
		spr->SetPivot(spr->GetTilingSize() * 0.5f);
		spr->SetScale(vec2(0.5f));
	}

	// setup the phases
	ph0 = AddPhase<EnemyCWPhase0>();
	ph1 = AddPhase<EnemyCWPhase1>();
	ph2 = AddPhase<EnemyCWPhase2>();
}

void BDEnemyCW::Update() { 
	BTEnemy::Update();
	Button b = Keyboard::GetKey(KeyCode::KeyG);
	if (b) {
		SetCurrentHealth(GetCurrentHealth() - 0.5f);
	}
}
