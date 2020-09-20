#include "BTPlayer.h"

RTTR_REGISTRATION{
	registration::class_<BTPlayer>("OWPlayer");
}

BTPlayer::BTPlayer() {
	maxHitpoints = 10;
	hitpoints = 10;

	BattleManager::AddPlayer(this);
}

BTPlayer::~BTPlayer() {
	BattleManager::RemovePlayer(this);
}