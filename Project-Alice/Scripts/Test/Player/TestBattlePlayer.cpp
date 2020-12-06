#include "TestBattlePlayer.hpp"

#include <iostream>

TestBattlePlayer::TestBattlePlayer() { }

TestBattlePlayer::~TestBattlePlayer() { }

void TestBattlePlayer::Start() {
	SetMoveSpeed(100.0f);
	SetMaxHitpoints(20, true);
	SetSprite("", vec2(8.0f, 8.0f), 2);
	SetCollider(8.0f);
	DEBUG_LOG("Player Pos is : " + VTOS(GetPosition()));

	// FOR TESTING //
	//SetBattleSpells(SpellInventory::GetEquippedAtkSpells(), SpellInventory::GetEquippedDefSpell());

	audioSource = AddComponent<AudioSourceComponent>();
	audioSource->SetSound("Test");
	audioSource->PlaySound(-1);
}