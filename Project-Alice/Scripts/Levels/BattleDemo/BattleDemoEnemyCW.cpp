#include "BattleDemoEnemyCW.hpp"

BDEnemyCW::BDEnemyCW() : ph0(nullptr), spr(nullptr) {

}

BDEnemyCW::~BDEnemyCW() { }

void BDEnemyCW::Start() {
	// add components
	if (spr = AddComponent<SpriteRenderer>()) {
		spr->LoadTexture("64by64texture");
	}

	// setup the phases
	ph0 = rttr_cast<EnemyCWPhase0*>(AddPhase(type::get<EnemyCWPhase0>()));
}
