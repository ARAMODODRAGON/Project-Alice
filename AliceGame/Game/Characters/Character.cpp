#include "Character.hpp"

Character::Character() : m_maxSpeed(55.0f), m_slowScalar(0.4f) { }

Character::~Character() { }

ALC::vec2 Character::GetInputAxis() const {
	using KM = ALC::Keyboard;
	using KC = ALC::KeyCode;
	auto up = KM::GetKey(KC::ArrowUp);
	auto down = KM::GetKey(KC::ArrowDown);
	auto left = KM::GetKey(KC::ArrowLeft);
	auto right = KM::GetKey(KC::ArrowRight);
	return ALC::vec2(
		static_cast<float>(right.IsHeld()) - static_cast<float>(left.IsHeld()),
		static_cast<float>(up.IsHeld()) - static_cast<float>(down.IsHeld())
	);
}

ALC::Button Character::GetShootButton() {
	return ALC::Keyboard::GetKey(ALC::KeyCode::KeyC);
}

ALC::Button Character::GetSlowButton() {
	return ALC::Keyboard::GetKey(ALC::KeyCode::LeftShift);
}

void Character::UpdateMovement(ALC::Entity self, ALC::Timestep ts) {
	auto inputAxis = GetInputAxis();
	auto slowinput = GetSlowButton().IsHeld();
	UpdateMovement(self, ts, inputAxis, (slowinput ? m_slowScalar : 1.0f) * m_maxSpeed);
}

void Character::UpdateMovement(ALC::Entity self, ALC::Timestep ts, const ALC::vec2& inputAxis, const float speed) {
	auto& body = self.GetComponent<ALC::CharacterBody>();
	auto& tr = self.GetComponent<ALC::Transform2D>();

	// update velocity
	if (glm::length2(inputAxis) > 0.0f)
		body.velocity = glm::normalize(inputAxis) * speed;
	else {
		body.velocity = ALC::vec2(0.0f);
		return; // didnt move so no need to check if out of bounds
	}

	// check if the position is not outside the level boundry
	ALC::rect cb;
	cb.min = (tr.position + body.velocity * ts) + body.radius;
	cb.max = (tr.position + body.velocity * ts) - body.radius;
	ALC::rect lb = BattleManager::GetDeathBounds();

	// outside horizontal
	if (cb.min.x > lb.max.x || cb.max.x < lb.min.x) {
		body.velocity.x = 0.0f;
		tr.position.x = ALC::Clamp(tr.position.x, lb.min.x, lb.max.x);
	}
	// outside vertical
	if (cb.min.y > lb.max.y || cb.max.y < lb.min.y) {
		body.velocity.y = 0.0f;
		tr.position.y = ALC::Clamp(tr.position.y, lb.min.y, lb.max.y);
	}
}

void Character::Start(ALC::Entity self) {
	if (!self.HasComponent<ALC::CharacterBody>()) {
		auto& cb = self.AddComponent<ALC::CharacterBody>();
		cb.radius = 4.0f;
	}
	if (!self.HasComponent<ALC::Transform2D>()) {
		auto& tr = self.AddComponent<ALC::Transform2D>();
		auto lb = BattleManager::GetDeathBounds();
		tr.position.x = 0.0f;
		tr.position.y = lb.min.y * 0.5f;
	}
	if (!self.HasComponent<ALC::SpriteComponent>()) {
		auto& spr = self.AddComponent<ALC::SpriteComponent>();
		spr.bounds = ALC::rect(-8.0f, -8.0f, 8.0f, 8.0f);
		spr.color = ALC_COLOR_RED;
	}
}
