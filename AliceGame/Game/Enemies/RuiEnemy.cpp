#include "RuiEnemy.hpp"

RuiEnemy::RuiEnemy() { }

RuiEnemy::~RuiEnemy() { }

void RuiEnemy::Start(ALC::Entity self) {
	Enemy::Start(self); // must be called to initialize

	auto& spr = self.GetComponent<ALC::SpriteComponent>();
	spr.texture = ALC::ContentManager::LoadTexture("Resources/Textures/Enemies/Rui-Facing-Forward.png");
	spr.textureBounds = spr.texture.GetBounds();

	// match the ratio 
	float ratio = spr.textureBounds.Height() / spr.textureBounds.Width();
	float newHeight = spr.bounds.Width() * ratio;
	spr.bounds.top = newHeight * 0.5f;
	spr.bounds.bottom = -newHeight * 0.5f;


}

void RuiEnemy::Update(ALC::Entity self, ALC::Timestep ts) {

}
