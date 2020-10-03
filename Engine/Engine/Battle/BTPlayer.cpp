#include "BTPlayer.hpp"
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
}

BTPlayer::~BTPlayer() {
	BattleManager::RemovePlayer(this);
}

void BTPlayer::Start() {
	sprite = AddComponent<Sprite>();
}

void BTPlayer::Update() {
	// Gather Keyboard Input for Movement
	const Button keyRight = Keyboard::GetKey(KeyCode::ArrowRight);
	const Button keyLeft = Keyboard::GetKey(KeyCode::ArrowLeft);
	const Button keyUp = Keyboard::GetKey(KeyCode::ArrowUp);
	const Button keyDown = Keyboard::GetKey(KeyCode::ArrowDown);

	// Set player velodity based on the current input
	vec2 inputDirection = vec2(float(keyRight.IsHeld()) - float(keyLeft.IsHeld()), float(keyUp.IsHeld()) - float(keyDown.IsHeld()));
	if (inputDirection.x != 0.0f && inputDirection.y != 0.0f) { // Don't normalize the vector if it's set to (0.0, 0.0)
		inputDirection = glm::normalize(inputDirection);
	}
	SetVelocity(inputDirection * moveSpeed);
	
	vec2 velocity = GetVelocity();
	std::cout << velocity.x << ", " << velocity.y << std::endl;
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

Sprite* BTPlayer::GetSprite() {
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

void BTPlayer::SetCollider(float _radius) {
	collider = AddComponent<CircleCollider>();
	collider->SetRadius(4.0f);
}