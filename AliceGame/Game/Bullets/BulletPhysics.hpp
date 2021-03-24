#ifndef ALC_BULLETS_BULLETPHYSICS_HPP
#define ALC_BULLETS_BULLETPHYSICS_HPP
#include <ALC/General.hpp>
#include <ALC/Layermask.hpp>
#include <ALC/Physics/CollisionInfo.hpp>
#include <ALC/Physics/Transform2D.hpp>

struct BCollisionInfo final {

	// layers that match in the collision
	ALC::Layermask32 GetCollisionMask() const { return m_collisionMask; }

	// layers that match in the trigger
	ALC::Layermask32 GetTriggerMask() const { return m_triggerMask; }

	// other entity involved in collision
	ALC::EntityID GetOther() const { return m_other; }

	// get the current collision state
	ALC::CollisionState GetState() const { return m_state; }

	float GetDamage() const { return m_damage; }

	BCollisionInfo() = default;
private:

	ALC::Layermask32 m_collisionMask = ALC::Layermask32::NONE;
	ALC::Layermask32 m_triggerMask = ALC::Layermask32::NONE;
	ALC::EntityID m_other;
	ALC::CollisionState m_state = ALC::CollisionState::None;
	float m_damage;

public:
	void __SetState(ALC::CollisionState state) { m_state = state; }
	BCollisionInfo(ALC::Layermask32 collisionMask, ALC::Layermask32 triggerMask, ALC::EntityID other, ALC::CollisionState state, float damage)
		: m_collisionMask(collisionMask), m_triggerMask(triggerMask), m_other(other), m_state(state), m_damage(damage) { }
};

struct BulletBody final {

	// is physics applied
	bool isSimulated = true;

	// velocity of the bullet
	ALC::vec2 velocity = ALC::vec2(0.0f);

	// radius of the bullet
	// physics does not apply if the radius is < 0.0001
	float radius = 0.5f;

	// what layers does this bullet collide with
	ALC::Layermask32 mask = ALC::Layermask32::ALL;

	// the damage output of this bullet
	float damage = 1.0f;

	// returns the last character that this bullet collided with
	ALC::EntityID GetLastCollision() const { return m_lastCollision; }

private:
	ALC::EntityID m_lastCollision = nullptr;
public:
	void __SetCol(ALC::EntityID id) { m_lastCollision = id; }
};

struct CharacterBody final {

	// is physics applied
	bool isSimulated = true;

	// velocity of the character
	ALC::vec2 velocity = ALC::vec2(0.0f);

	// radius of the character
	// physics does not apply if the radius is < 0.0001
	float radius = 0.5f;

	// what layers does this character collide with
	ALC::Layermask32 mask = ALC::Layermask32::ALL;

	// iterator used for iterating the collision list
	using const_iterator = ALC::vector<BCollisionInfo>::const_iterator;

	// iterator pointing to the begining of the collision list
	const_iterator begin() const { return m_collisions.begin(); }

	// iterator pointing to the end of the collision list
	const_iterator end() const { return m_collisions.end(); }

	// returns the number of collisions 
	size_t Count() const { return m_collisions.size(); }

	// returns collision at index
	const BCollisionInfo& operator[](const size_t index) const { return m_collisions[index]; }

private:
	ALC::vector<BCollisionInfo> m_collisions;
public:
	ALC::vector<BCollisionInfo>& __GetColl() ALC_INTERNAL { return m_collisions; }
};


#endif // !ALC_BULLETS_BULLETPHYSICS_HPP