#include "AliceChara.hpp"

AliceChara::AliceChara() { }

AliceChara::~AliceChara() { }

void AliceChara::Start(ALC::Entity self) {
	// call base
	Character::Start(self);
}

void AliceChara::Update(ALC::Entity self, ALC::Timestep ts) {
	Character::UpdateMovement(self, ts);
}

void AliceChara::LateUpdate(ALC::Entity self, ALC::Timestep ts) { 
	Character::UpdateColliderSprite(self, ts);
}
