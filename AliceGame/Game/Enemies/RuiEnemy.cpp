#include "RuiEnemy.hpp"

RuiEnemy::RuiEnemy() { }

RuiEnemy::~RuiEnemy() { }

void RuiEnemy::Start(ALC::Entity self) {
	Enemy::Start(self); // must be called to initialize
}

void RuiEnemy::Update(ALC::Entity self, ALC::Timestep ts) {

}
