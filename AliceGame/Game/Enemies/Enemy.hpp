#ifndef ALICE_ENEMIES_ENEMY_HPP
#define ALICE_ENEMIES_ENEMY_HPP
#include <ALC\Bullets.hpp>
#include <ALC\Input.hpp>
#include <ALC\Rendering.hpp>
#include <ALC\Content.hpp>
#include "../BattleManager.hpp"

class Enemy : public ALC::ShooterBehavior {
public:

	Enemy();
	virtual ~Enemy() = 0;

	// call this to update the characters movement
	// and clamp them within the BattleManager::GetLevelBounds()
	void UpdateMovement(ALC::Entity self, ALC::Timestep ts, const ALC::vec2& velocity);

	// garuntees that this entity has a character body, transform, and spritecomponent
	virtual void Start(ALC::Entity self);

private:
};

#endif // !ALICE_ENEMIES_ENEMY_HPP