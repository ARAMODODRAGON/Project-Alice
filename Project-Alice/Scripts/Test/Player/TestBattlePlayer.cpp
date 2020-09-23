#include "TestBattlePlayer.hpp"

#include <iostream>

RTTR_REGISTRATION{
	registration::class_<TestBattlePlayer>("TestBattlePlayer")
		.public_object_constructor;
}

TestBattlePlayer::TestBattlePlayer() { }

TestBattlePlayer::~TestBattlePlayer() { }

void TestBattlePlayer::Start() {
	SetMaxHitpoints(20);
	SetHitpoints(20);
}

void TestBattlePlayer::Update() {

}

void TestBattlePlayer::LateUpdate() {

}