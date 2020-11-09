#include "BTPlayer.hpp"
#include "Spells/Attack/BTAttackSpell.hpp"
#include "Spells/Defence/BTDefenceSpell.hpp"
#include <iostream>

RTTR_REGISTRATION{
	registration::class_<BTPlayer>("BTPlayer")
		.property("moveSpeed", &BTPlayer::moveSpeed)
		.property("hitpoints", &BTPlayer::hitpoints)
		.property("maxHitpoints", &BTPlayer::maxHitpoints);
}

BTPlayer::BTPlayer() {
	BattleManager::AddPlayer(this);

	moveSpeed = 0.0f;

	maxHitpoints = 0;
	hitpoints = 0;

	sprite = nullptr;
	collider = nullptr;

	defenceSpell = nullptr;
	atkSpells = { nullptr, nullptr, nullptr };
	curAtkSpell = 0;
}

BTPlayer::~BTPlayer() {
	BattleManager::RemovePlayer(this);
}

void BTPlayer::Start() {
	
}

void BTPlayer::Update() {
	// Gather Keyboard Input for Movement
	const Button keyRight = Keyboard::GetKey(KeyCode::ArrowRight);
	const Button keyLeft = Keyboard::GetKey(KeyCode::ArrowLeft);
	const Button keyUp = Keyboard::GetKey(KeyCode::ArrowUp);
	const Button keyDown = Keyboard::GetKey(KeyCode::ArrowDown);

	const Button keyPrevSpell = Keyboard::GetKey(KeyCode::KeyZ);
	const Button keyNextSpell = Keyboard::GetKey(KeyCode::KeyX);
	const Button keyAtkSpell = Keyboard::GetKey(KeyCode::Space);
	const Button keyDefSpell = Keyboard::GetKey(KeyCode::KeyV);

	// Set player velodity based on the current input
	vec2 inputDirection = vec2(float(keyRight.IsHeld()) - float(keyLeft.IsHeld()), float(keyUp.IsHeld()) - float(keyDown.IsHeld()));
	if (inputDirection.x != 0.0f && inputDirection.y != 0.0f) { // Don't normalize the vector if it's set to (0.0, 0.0)
		inputDirection = glm::normalize(inputDirection);
	}
	SetVelocity(inputDirection * moveSpeed);
	
	// Swapping to the next/previous spell
	if (keyPrevSpell.Pressed()) {
		curAtkSpell--;
		if (curAtkSpell < 0) {
			curAtkSpell = MAX_EQUIPPED_SPELLS - 1;
		}
	}
	else if (keyNextSpell.Pressed()) {
		curAtkSpell++;
		if (curAtkSpell > MAX_EQUIPPED_SPELLS - 1) {
			curAtkSpell = 0;
		}
	}

	// Using the current Attacking Spell
	if (atkSpells[curAtkSpell] != nullptr) {
		atkSpells[curAtkSpell]->Update(keyAtkSpell);
	}

	if (keyDefSpell.Pressed() && defenceSpell != nullptr) {
		defenceSpell->Use();
	}
}

void BTPlayer::LateUpdate() {

}

// Increases or decreases the player's hitpoints by the value of the modifier; maxing out the hp at its maximum possible value
// and killing the player when it reaches zero.
void BTPlayer::UpdateHitpoints(int _modifier) {
	hitpoints += _modifier;
	if (hitpoints > maxHitpoints) {
		hitpoints = maxHitpoints;
	} else if (hitpoints <= 0) {
		hitpoints = 0;
		// TODO -- Make player dead here
	}
}

float BTPlayer::GetMoveSpeed() {
	return moveSpeed;
}

int BTPlayer::GetHitpoints() {
	return hitpoints;
}

int BTPlayer::GetMaxHitpoints() {
	return maxHitpoints;
}

SpriteRenderer* BTPlayer::GetSprite() {
	return sprite;
}

CircleCollider* BTPlayer::GetCollider() {
	return collider;
}

void BTPlayer::SetMoveSpeed(float _moveSpeed) {
	moveSpeed = _moveSpeed;
}

void BTPlayer::SetHitpoints(int _hitpoints) {
	UpdateHitpoints(_hitpoints - hitpoints);
}

void BTPlayer::SetMaxHitpoints(int _maxHitpoints, bool _updateCurHP) {
	maxHitpoints = _maxHitpoints;
	if (_updateCurHP) { // Optionally, the current HP can be maxed out
		hitpoints = _maxHitpoints;
	}
}

void BTPlayer::SetSprite(const string& _texture, vec2 _pivot, int _layer) {
	sprite = AddComponent<SpriteRenderer>();
	if (_texture != "") { // Only load a texture if one is specified
		sprite->LoadTexture(_texture);
	}
	sprite->SetPivot(_pivot);
	sprite->SetLayer(_layer);
}

void BTPlayer::SetCollider(float _radius) {
	collider = AddComponent<CircleCollider>();
	collider->SetRadius(4.0f);
}

void BTPlayer::SetBattleSpells(array<string, MAX_EQUIPPED_SPELLS> _atkSpells, const string& _defSpell) {
	// Add the three equipped attack spells
	for (uint32 i = 0; i < MAX_EQUIPPED_SPELLS; i++) {
		type t = type::get_by_name(_atkSpells[i].c_str());
		variant spell = t.create();
		if (!spell) { // The spell couldn't be created; throw an error.
			DEBUG_ERROR("Spell " + _atkSpells[i] + " couldn't be created!");
			continue;
		}
		atkSpells[i] = spell.get_value<BTAttackSpell*>();
	}
	// Add the single equipped defence spell
	type t = type::get_by_name(_defSpell.c_str());
	variant spell = t.create();
	if (!spell) { // The spell couldn't be created; throw an error.
		DEBUG_ERROR("Spell " + _defSpell + " couldn't be created!");
		return;
	}
	defenceSpell = spell.get_value<BTDefenceSpell*>();
}