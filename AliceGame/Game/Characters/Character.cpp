#include "Character.hpp"

#define INVULN_FLASH_SPEED_SCALAR (1.0f / 0.06f)

Character::Character()
	: m_health(3.0f)
	, m_invuln(0.0f)
	, m_shouldFlashOnInvuln(true)
	, m_invulnOnHit(1.0f)
	, m_maxSpeed(90.0f)
	, m_slowScalar(0.4f) { }

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

void Character::TakeDamage(const float damage) {
	// call the other function, require 'self' to invoke virtual functions
	auto self = GetRegistry().GetEntity(GetEntityID());
	TakeDamage(self, damage);
}

void Character::TakeDamage(ALC::Entity self, const float damage) {
	const bool wasDead = IsDead();
	// take damage
	OnTakeDamage(self, damage);

	// on die
	if (IsDead() && !wasDead) OnDeath(self);
}

void Character::UpdateCollisions(ALC::Entity self, ALC::Timestep ts) {
	// character was invulnerable, ignore collisions
	if (IsInvuln()) {
		if (m_invuln > 0.0f) m_invuln -= ts;
		if (m_invuln < 0.0f) m_invuln = 0.0f;
		return;
	}

	auto& cb = self.GetComponent<ALC::CharacterBody>();

	// take damage once if there was a collision
	// then do invuln
	if (m_invulnOnHit > 0.0f) {
		if (cb.Count() > 0) {
			TakeDamage(self, 1.0f);
			if (!IsDead()) {
				m_invuln -= ts;
				if (m_invuln < 0.0f) m_invuln = 0.0f;
			}
		}

		if (m_invuln > 0.0f) m_invuln -= ts;
		if (m_invuln < 0.0f) m_invuln = 0.0f;
	}

	// take damage for each collision
	else {
		for (auto& cinfo : cb) {
			TakeDamage(self, 1.0f);
			if (IsDead()) break;
		}
	}
}

void Character::OnTakeDamage(ALC::Entity self, const float damage) {
	if ((m_health -= damage) < 0.0f) m_health = 0.0f;
	if (m_invulnOnHit > 0.0f) m_invuln = m_invulnOnHit;
}

ALC::Entity Character::GetColliderSprite() {
	return GetRegistry().GetEntity(m_colliderEntity);
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
	ALC::rect lb = BattleManager::GetLevelBounds();

	// outside horizontal
	if (cb.min.x > lb.max.x || cb.max.x < lb.min.x) {
		tr.position.x += body.velocity.x;
		body.velocity.x = 0.0f;
		tr.position.x = ALC::Clamp(tr.position.x, lb.min.x + body.radius, lb.max.x - body.radius);
	}
	// outside vertical
	if (cb.min.y > lb.max.y || cb.max.y < lb.min.y) {
		tr.position.y += body.velocity.y;
		body.velocity.y = 0.0f;
		tr.position.y = ALC::Clamp(tr.position.y, lb.min.y + body.radius, lb.max.y - body.radius);
	}
}

void Character::Start(ALC::Entity self) {
	using CM = ALC::ContentManager;

	// make sure to add the components *before* we get the components
	// if we dont then the positions of the components in memory can change which messes the references

	// create the collider's sprite first so it is visible on top of the character
	auto collEntity = GetRegistry().Create();
	m_colliderEntity = collEntity.GetComponent<ALC::EntityInfo>().GetID();
	collEntity.AddComponent<ALC::Transform2D>();
	collEntity.AddComponent<ALC::SpriteComponent>();

	// add the required components
	if (!self.HasComponent<ALC::CharacterBody>())	self.AddComponent<ALC::CharacterBody>();
	if (!self.HasComponent<ALC::Transform2D>())		self.AddComponent<ALC::Transform2D>();
	if (!self.HasComponent<ALC::SpriteComponent>())	self.AddComponent<ALC::SpriteComponent>();

	// get our components only after 'adding' them

	auto& cb = self.GetComponent<ALC::CharacterBody>();
	auto& tr = self.GetComponent<ALC::Transform2D>();
	auto& spr = self.GetComponent<ALC::SpriteComponent>();
	auto& tr0 = collEntity.GetComponent<ALC::Transform2D>();
	auto& spr0 = collEntity.GetComponent<ALC::SpriteComponent>();

	// initalize 
	cb.radius = 1.0f;
	auto lb = BattleManager::GetLevelBounds();
	tr.position.x = 0.0f;
	tr.position.y = lb.min.y * 0.5f;
	spr.bounds = ALC::rect(8.0f);

	tr0.position = tr.position; // place on top of character
	spr0.bounds = ALC::rect(cb.radius); // match size of character collider
	spr0.color = ALC_COLOR_BLUE;
	spr0.texture = CM::LoadTexture("Resources/Textures/circle.png");
	spr0.textureBounds = spr0.texture.GetBounds();
}

void Character::UpdateSprites(ALC::Entity self, ALC::Timestep ts) {
	auto [cb, tr, spr] = self.GetComponent<ALC::CharacterBody, ALC::Transform2D, ALC::SpriteComponent>();

	if (m_shouldFlashOnInvuln) {
		// update our sprite accordingly
		if (m_invuln > 0.0f)
			// [abs(tan(t) * 0.7)] produces a nice flashing animation
			// [0.4 * abs(tan(t) * 0.7) + 0.6] scales it so it ranges from 0.6 to 1
			spr.color.a = 0.4f * fabs(tanf(m_invuln * INVULN_FLASH_SPEED_SCALAR) * 0.7f) + 0.6f;
		else
			spr.color.a = 1.0f;
	}

	// update the collider entity
	if (auto collEntity = GetRegistry().GetEntity(m_colliderEntity)) {

		auto& tr0 = collEntity.GetComponent<ALC::Transform2D>();
		tr0.position = tr.position; // place on top of character

		auto& spr0 = collEntity.GetComponent<ALC::SpriteComponent>();
		spr0.bounds = ALC::rect(cb.radius); // match size of character collider

	} else ALC_DEBUG_WARNING("Collider entity not found");
}
