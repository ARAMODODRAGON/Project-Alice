#include "BTPlayer.h"

RTTR_REGISTRATION{
	registration::class_<BTPlayer>("BTPlayer")
		.property("hitpoints", &BTPlayer::hitpoints)
		.property("maxHitpoints", &BTPlayer::maxHitpoints);
}

BTPlayer::BTPlayer() {
	BattleManager::AddPlayer(this);

	maxHitpoints = 0;
	hitpoints = 0;

	sprite = nullptr;
}

BTPlayer::~BTPlayer() {
	BattleManager::RemovePlayer(this);
}

void BTPlayer::Start() {

}

void BTPlayer::Update() {

}

void BTPlayer::LateUpdate() {

}

void BTPlayer::UpdateHitpoints(int _modifier) {
	hitpoints += _modifier;
	if (hitpoints > maxHitpoints) {
		hitpoints = maxHitpoints;
	} else if (hitpoints <= 0) {
		hitpoints = 0;
		// TODO -- Make player dead here
	}
}