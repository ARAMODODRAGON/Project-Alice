#ifndef ALICE_CHARACTERS_CHARACTER_HPP
#define ALICE_CHARACTERS_CHARACTER_HPP
#include <ALC\Bullets.hpp>
#include <ALC\Input.hpp>
#include <ALC\Rendering.hpp>
#include <ALC\Content.hpp>
#include <ALC\StateMachine.hpp>
#include "../BattleManager.hpp"

class Character : public ALC::ShooterBehavior {
public:

	Character();
	virtual ~Character() = 0;

	ALC::vec2 GetInputAxis() const;
	ALC::Button GetShootButton() const;
	ALC::Button GetSlowButton() const;
	ALC::Button GetDefButton() const;
	ALC::Button GetModButton() const;

	float GetMaxSpeed() const { return m_maxSpeed; }
	void SetMaxSpeed(const float maxSpeed) { m_maxSpeed = maxSpeed; }

	float GetSlowScalar() const { return m_slowScalar; }
	void SetSlowScalar(const float slowScalar) { m_slowScalar = slowScalar; }

	float GetHealth() const { return m_health; }
	void SetHealth(const float health) { m_health = health; }
	
	float GetShouldFlashOnInvuln() const { return m_shouldFlashOnInvuln; }
	void SetShouldFlashOnInvuln(const float shouldFlashOnInvuln) { m_shouldFlashOnInvuln = shouldFlashOnInvuln; }

	float GetInvuln() const { return m_invuln; }
	void SetInvuln(const float invuln) { m_invuln = invuln; }

	// returns true if this player is dead
	bool IsDead() const { return ALC::NearlyZero(m_health); }

	// returns true if this player is invulnerable
	bool IsInvuln() const { return m_invuln > 0.0f; }

	// makes the player take damage
	void TakeDamage(const float damage = 1.0f);

	// iterates through the collisions and invokes OnTakeDamage(...) for each if the player should take damage
	// also iterates the invulnerability counter
	void UpdateCollisions(ALC::Entity self, ALC::Timestep ts);

	// called for all damage taken, call base to update health and invuln automatically
	virtual void OnTakeDamage(ALC::Entity self, const float damage);

	// called on death, always after OnTakeDamage(...)
	virtual void OnDeath(ALC::Entity self) { }

	// returns the entity that the collider sprite is attached to
	ALC::Entity GetColliderSprite();

	// call this to update the characters movement
	// and clamp them within the BattleManager::GetLevelBounds()
	void UpdateMovement(ALC::Entity self, ALC::Timestep ts);

	// alternate call with more inputs
	void UpdateMovement(ALC::Entity self, ALC::Timestep ts, const ALC::vec2& inputAxis, const float speed);

	// garuntees that this entity has a character body, transform, and spritecomponent + a collider sprite entity
	virtual void Start(ALC::Entity self);

	// updates the collider sprite entity to match this characters position
	// updates this entity's sprite to match the invulnerability
	void UpdateSprites(ALC::Entity self, ALC::Timestep ts);

private:

	void TakeDamage(ALC::Entity self, const float damage = 1.0f);

	float m_health;
	float m_invuln;
	bool m_shouldFlashOnInvuln;
	float m_invulnOnHit;
	float m_maxSpeed;
	float m_slowScalar;
	ALC::EntityID m_colliderEntity;
};

#endif // !ALICE_CHARACTERS_CHARACTER_HPP