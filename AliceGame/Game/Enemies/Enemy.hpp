#ifndef ALICE_ENEMIES_ENEMY_HPP
#define ALICE_ENEMIES_ENEMY_HPP
#include <ALC\Bullets.hpp>
#include <ALC\Input.hpp>
#include <ALC\Rendering.hpp>
#include <ALC\Content.hpp>
#include "../BattleManager.hpp"
#include "../BTA.hpp"
#include "../Systems/MoveStates.hpp"





class Enemy : public ALC::ShooterBehavior {
public:

	Enemy();
	virtual ~Enemy() = 0;

	// call to mark this enemy as "done" so the post battle cutscene can start
	void MarkDone() { m_isDone = true; }

	// returns if this is "done" otherwise meaning that this enemy has been killed
	bool IsDone() const { return m_isDone; }

	float GetHealth() const { return m_health; }
	void SetHealth(const float health) { m_health = health; }

	// updates health and max health to the given value
	void ResetHealth(const float health) {
		m_health = m_maxHealth = health;
	}
	float GetMaxHealth() const { return m_maxHealth; }

	float GetLifetime() const { return m_lifetime; }
	void SetLifetime(const float lifetime) { m_lifetime = lifetime; }

	void SetSpeed(float _speed) { m_speed = _speed; }
	float GetSpeed() { return m_speed; }

	void SetAccel(float _accel) { m_acceleration = _accel; }
	float GetAccel() { return m_acceleration; }

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

	// called by the scene when a battle begins
	virtual void BattleBegin() = 0;

	// call this to update the characters movement
	// and clamp them within the BattleManager::GetLevelBounds()
	void UpdateMovement(ALC::Entity self, ALC::Timestep ts, const ALC::vec2& velocity);

	// garuntees that this entity has a character body, transform, and spritecomponent
	virtual void Start(ALC::Entity self);


	



private:

	void TakeDamage(ALC::Entity self, const float damage = 1.0f);

	float m_health;
	float m_maxHealth;
	float m_lifetime;
	float m_acceleration;
	float m_speed;
	bool m_isDone;
protected:
	MoveStates m_moveState;

};

#endif // !ALICE_ENEMIES_ENEMY_HPP