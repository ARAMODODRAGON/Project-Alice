#include "Enemy.hpp"

Enemy::Enemy() : m_health(1000.0f), m_maxHealth(1000.0f), m_lifetime(-1.0f), m_isDone(false) {
	ShooterBehavior::SetDefaultCollisionmask(BTL_PLAYERMASK);
	ShooterBehavior::SetDefaultSpriteLayer(SPRL::ENEMY_BULLET);
	BattleManager::AddEnemy(this);
}

Enemy::~Enemy() {
	BattleManager::RemoveEnemy(this);
}

void Enemy::TakeDamage(const float damage) {
	// call the other function, require 'self' to invoke virtual functions
	auto self = GetRegistry().GetEntity(GetEntityID());
	TakeDamage(self, damage);
}

void Enemy::TakeDamage(ALC::Entity self, const float damage) {
	const bool wasDead = IsDead();
	// take damage
	OnTakeDamage(self, damage);

	// on die
	if ((m_health <= 0.0f) && !wasDead) OnDeath(self);
}


void Enemy::UpdateCollisions(ALC::Entity self, ALC::Timestep ts) {
	auto& cb = self.GetComponent<CharacterBody>();

	// take damage for each collision
	for (auto& cinfo : cb) {
		// only take damage on begin collision
		if (cinfo.GetState() == ALC::CollisionState::Begin) {
			TakeDamage(self, cinfo.GetDamage());
			if (m_health <= 0.0f) break;
		}
	}
}

void Enemy::OnTakeDamage(ALC::Entity self, const float damage) {
	if ((m_health -= damage) <= 0.0f) m_health = 0.0f;
}

void Enemy::UpdateLifetime(ALC::Entity self, ALC::Timestep ts, const float scalar) {
	bool wasDead = IsDead();
	if ((m_lifetime -= ts * scalar) <= 0.0f) m_lifetime = 0.0f;
	if ((m_lifetime <= 0.0f) && !wasDead) OnDeath(self);
}

void Enemy::UpdateMovement(ALC::Entity self, ALC::Timestep ts, const ALC::vec2& velocity) {
	auto& body = self.GetComponent<CharacterBody>();
	auto& tr = self.GetComponent<ALC::Transform2D>();

	// update velocity
	if (glm::length2(velocity) > 0.0f)
		body.velocity = velocity;
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
		tr.position.x = glm::clamp(tr.position.x, lb.min.x + body.radius, lb.max.x - body.radius);
	}
	// outside vertical
	if (cb.min.y > lb.max.y || cb.max.y < lb.min.y) {
		tr.position.y += body.velocity.y;
		body.velocity.y = 0.0f;
		tr.position.y = ALC::Clamp(tr.position.y, lb.min.y + body.radius, lb.max.y - body.radius);
	}
}

void Enemy::Start(ALC::Entity self) {
	using CM = ALC::ContentManager;

	// add the required components
	if (!self.HasComponent<CharacterBody>())	self.AddComponent<CharacterBody>();
	if (!self.HasComponent<ALC::Transform2D>())		self.AddComponent<ALC::Transform2D>();
	if (!self.HasComponent<ALC::SpriteComponent>())	self.AddComponent<ALC::SpriteComponent>();

	// initalize components
	auto [cb, tr, spr] = self.GetComponent<CharacterBody, ALC::Transform2D, ALC::SpriteComponent>();

	cb.radius = 20.0f;
	cb.mask = BTL_ENEMYMASK;
	auto lb = BattleManager::GetLevelBounds();
	tr.position.x = 0.0f;
	tr.position.y = lb.max.y * 0.5f;
	spr.bounds = ALC::rect(20.0f);
	spr.layer = SPRL::ENEMY;

}
