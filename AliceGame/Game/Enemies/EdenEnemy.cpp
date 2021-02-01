#include "EdenEnemy.hpp"

EdenEnemy::EdenEnemy() { }

EdenEnemy::~EdenEnemy() { }

void EdenEnemy::Start(ALC::Entity self) {
	Enemy::Start(self);
}

void EdenEnemy::Update(ALC::Entity self, ALC::Timestep ts) {
	Enemy::Update(self, ts);
}

void EdenEnemy::LateUpdate(ALC::Entity self, ALC::Timestep ts) {
	Enemy::LateUpdate(self, ts);
}

void EdenEnemy::OnDeath(ALC::Entity self) {
	Enemy::OnDeath(self);
}

void EdenEnemy::BattleBegin() { }
