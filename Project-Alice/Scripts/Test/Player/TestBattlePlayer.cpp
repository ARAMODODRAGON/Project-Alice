#include "TestBattlePlayer.hpp"

#include <iostream>

RTTR_REGISTRATION{
	registration::class_<TestBattlePlayer>("TestBattlePlayer")
		.public_object_constructor;
}

TestBattlePlayer::TestBattlePlayer() { }

TestBattlePlayer::~TestBattlePlayer() { }

void TestBattlePlayer::Start() {
	SetMoveSpeed(100.0f);
	SetMaxHitpoints(20, true);
	SetCollider(8.0f);
}