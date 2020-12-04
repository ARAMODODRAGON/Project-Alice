#include "BTPlayer.hpp"
#include "Spells/Attack/BTAttackSpell.hpp"
#include "Spells/Defence/BTDefenceSpell.hpp"
#include <iostream>

BTPlayer::BTPlayer() {
	BattleManager::AddPlayer(this);

	moveSpeed = 0.0f;
	slowSpeed = 0.0f;

	maxHitpoints = 0;
	hitpoints = 0;

	if (sprite = AddComponent<SpriteRenderer>()) { }
	else DEBUG_ERROR("Failed to add SpriteRenderer to player");
	if (collider = AddComponent<CircleCollider>()) {

	}
	else DEBUG_ERROR("Failed to add CircleCollider to player");

	defenceSpell = nullptr;
	atkSpells = { nullptr, nullptr, nullptr };
	curAtkSpell = 0;
}

BTPlayer::~BTPlayer() {
	DeleteSpells();
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
	const Button keySlow = Keyboard::GetKey(KeyCode::LeftShift);

	const Button keyPrevSpell = Keyboard::GetKey(KeyCode::KeyZ);
	const Button keyNextSpell = Keyboard::GetKey(KeyCode::KeyX);
	const Button keyAtkSpell = Keyboard::GetKey(KeyCode::Space);
	const Button keyDefSpell = Keyboard::GetKey(KeyCode::KeyV);

	// Set player velodity based on the current input
	vec2 inputDirection = vec2(float(keyRight.IsHeld()) - float(keyLeft.IsHeld()), float(keyUp.IsHeld()) - float(keyDown.IsHeld()));
	if (glm::length2(inputDirection) > 0.0f) { // Don't normalize the vector if it's set to (0.0, 0.0)
		inputDirection = glm::normalize(inputDirection);
	}
	SetVelocity(inputDirection * (keySlow ? slowSpeed : moveSpeed));

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
	Rect r = BattleManager::GetBattleArea();

	r.min += vec2(3);
	r.max -= vec2(3);
	SetPosition(r.Clamp(GetPosition()));
}

// Increases or decreases the player's hitpoints by the value of the modifier; maxing out the hp at its maximum possible value
// and killing the player when it reaches zero.
void BTPlayer::UpdateHitpoints(int _modifier) {
	hitpoints += _modifier;
	if (hitpoints > maxHitpoints) {
		hitpoints = maxHitpoints;
	}
	else if (hitpoints <= 0) {
		hitpoints = 0;
		// TODO -- Make player dead here
	}
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

void BTPlayer::SetBattleSpells(ATKSpells atkSpells_, BTDefenceSpell* defenceSpell_) {
	DeleteSpells();
	atkSpells = atkSpells_;
	defenceSpell = defenceSpell_;
}

void BTPlayer::DeleteSpells() {
	for (BTAttackSpell* sp : atkSpells) {
		if (sp) delete sp, sp = nullptr;
	}
	if (defenceSpell) delete defenceSpell, defenceSpell = nullptr;
}