#include "Enemy.hpp"

Enemy::Enemy() { }

Enemy::~Enemy() { }

void Enemy::UpdateMovement(ALC::Entity self, ALC::Timestep ts, const ALC::vec2& velocity) { }

void Enemy::Start(ALC::Entity self) {
	using CM = ALC::ContentManager;

	// add the required components
	if (!self.HasComponent<ALC::CharacterBody>())	self.AddComponent<ALC::CharacterBody>();
	if (!self.HasComponent<ALC::Transform2D>())		self.AddComponent<ALC::Transform2D>();
	if (!self.HasComponent<ALC::SpriteComponent>())	self.AddComponent<ALC::SpriteComponent>();

	// initalize components
	auto [cb, tr, spr] = self.GetComponent<ALC::CharacterBody, ALC::Transform2D, ALC::SpriteComponent>();

	cb.radius = 4.0f;
	auto lb = BattleManager::GetLevelBounds();
	tr.position.x = 0.0f;
	tr.position.y = lb.max.y * 0.5f;
	spr.bounds = ALC::rect(8.0f);
	spr.color = ALC_COLOR_BLUE;

}
