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

	float GetHealth() const { return m_health; }
	void SetHealth(const float health) { m_health = health; }

	float GetLifetime() const { return m_lifetime; }
	void SetLifetime(const float lifetime) { m_lifetime = lifetime; }

	// returns true if this enemy is dead
	bool IsDead() const { return ALC::NearlyZero(m_health) || ALC::NearlyZero(m_lifetime); }

	// makes the enemy take damage
	void TakeDamage(const float damage = 1.0f);

	// iterates through the collisions and invokes OnTakeDamage(...) for each bullet
	void UpdateCollisions(ALC::Entity self, ALC::Timestep ts);

	// called for all damage taken, call base to update health automatically
	virtual void OnTakeDamage(ALC::Entity self, const float damage);

	// called on death, always after OnTakeDamage(...) unless refering to the timer
	virtual void OnDeath(ALC::Entity self) { }

	// updates lifetime, calls OnDeath(...) when timer == 0
	void UpdateLifetime(ALC::Entity self, ALC::Timestep ts, const float scalar = 1.0f);

	// call this to update the characters movement
	// and clamp them within the BattleManager::GetLevelBounds()
	void UpdateMovement(ALC::Entity self, ALC::Timestep ts, const ALC::vec2& velocity);

	// garuntees that this entity has a character body, transform, and spritecomponent
	virtual void Start(ALC::Entity self);

private:

	void TakeDamage(ALC::Entity self, const float damage = 1.0f);

	float m_health;
	float m_lifetime;
};

#endif // !ALICE_ENEMIES_ENEMY_HPP