#include "BDPlayer.hpp"
#include "Engine\Battle\Spells\Attack\BTAttackSpell.hpp"

BDPlayer::BDPlayer() {
	SetMoveSpeed(70.0f);
	SetSlowSpeed(25.0f);
	GetSprite()->SetScale(vec2(32.0f));
	GetSprite()->SetPivot(vec2(0.5f));
	//SetBattleSpells({ }, nullptr);
}

BDPlayer::~BDPlayer() {
}

void BDPlayer::Start() {
	BTPlayer::Start();
}

void BDPlayer::Update() {
	// moves player
	BTPlayer::Update();
}

void BDPlayer::LateUpdate() {
	// clamps player within the battle area
	BTPlayer::LateUpdate();
}
