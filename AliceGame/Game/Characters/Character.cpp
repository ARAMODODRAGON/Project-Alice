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
	// create the collider's sprite first so it is visible on top of the character
	auto collEntity = GetRegistry().Create();
	m_colliderEntity = collEntity.GetComponent<ALC::EntityInfo>().GetID();
	auto& tr0 = collEntity.AddComponent<ALC::Transform2D>();
	auto& spr0 = collEntity.AddComponent<ALC::SpriteComponent>();

	using CM = ALC::ContentManager;
	// add the required components
	if (!self.HasComponent<ALC::CharacterBody>())	self.AddComponent<ALC::CharacterBody>();
	if (!self.HasComponent<ALC::Transform2D>())		self.AddComponent<ALC::Transform2D>();
	if (!self.HasComponent<ALC::SpriteComponent>())	self.AddComponent<ALC::SpriteComponent>();

	// initalize components
	auto [cb, tr, spr] = self.GetComponent<ALC::CharacterBody, ALC::Transform2D, ALC::SpriteComponent>();

	cb.radius = 4.0f;
	auto lb = BattleManager::GetDeathBounds();
	tr.position.x = 0.0f;
	tr.position.y = lb.min.y * 0.5f;
	spr.bounds = ALC::rect(8.0f);
	spr.color = ALC_COLOR_RED;

	tr0.position = tr.position; // place on top of character
	spr0.bounds = ALC::rect(cb.radius); // match size of character collider
	spr0.color = ALC_COLOR_YELLOW;
	spr0.texture = CM::LoadTexture("Resources/Textures/circle.png");
	spr0.textureBounds = spr0.texture.GetBounds();
}

void Character::UpdateColliderSprite(ALC::Entity self, ALC::Timestep ts) {
	// update the collider entity
	if (auto collEntity = GetRegistry().GetEntity(m_colliderEntity)) {
		auto [cb, tr, spr] = self.GetComponent<ALC::CharacterBody, ALC::Transform2D, ALC::SpriteComponent>();

		auto& tr0 = collEntity.GetComponent<ALC::Transform2D>();
		tr0.position = tr.position; // place on top of character

		auto& spr0 = collEntity.GetComponent<ALC::SpriteComponent>();
		spr0.bounds = ALC::rect(cb.radius); // match size of character collider

	} else ALC_DEBUG_WARNING("Collider entity not found");
}
