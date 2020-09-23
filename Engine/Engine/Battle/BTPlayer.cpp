#include "BTPlayer.hpp"

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

	// Set player velodity based on the current input
	vec2 inputDirection = normalize(vec2((keyRight.IsHeld() - keyLeft.IsHeld(), keyUp.IsHeld() - keyDown.IsHeld())));
	SetVelocity(inputDirection * moveSpeed);
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

void BTPlayer::SetSprite(std::string _filepath) {
	sprite = AddComponent<Sprite>();
	sprite->SetPivot(vec2(8.0f, 8.0f));
}