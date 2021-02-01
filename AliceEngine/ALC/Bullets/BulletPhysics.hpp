#ifndef ALC_BULLETS_BULLETPHYSICS_HPP
#define ALC_BULLETS_BULLETPHYSICS_HPP
#include "../General.hpp"
#include "../Layermask.hpp"
#include "../Physics/CollisionInfo.hpp"
#include "../Physics/Transform2D.hpp"

namespace ALC {

	struct BCollisionInfo final {

		// layers that match in the collision
		Layermask32 GetCollisionMask() const { return m_collisionMask; }

		// layers that match in the trigger
		Layermask32 GetTriggerMask() const { return m_triggerMask; }

		// other entity involved in collision
		EntityID GetOther() const { return m_other; }

		// get the current collision state
		CollisionState GetState() const { return m_state; }

		float GetDamage() const { return m_damage; }

		BCollisionInfo() = default;
	private:

		Layermask32 m_collisionMask = Layermask32::NONE;
		Layermask32 m_triggerMask = Layermask32::NONE;
		EntityID m_other;
		CollisionState m_state = CollisionState::None;
		float m_damage;

	public:
		void __SetState(CollisionState state) { m_state = state; }
		BCollisionInfo(Layermask32 collisionMask, Layermask32 triggerMask, EntityID other, CollisionState state, float damage) ALC_INTERNAL
			: m_collisionMask(collisionMask), m_triggerMask(triggerMask), m_other(other), m_state(state), m_damage(damage) { }
	};

	struct BulletBody final {

		// is physics applied
		bool isSimulated = true;

		// velocity of the bullet
		vec2 velocity = vec2(0.0f);

		// radius of the bullet
		// physics does not apply if the radius is < 0.0001
		float radius = 0.5f;

		// what layers does this bullet collide with
		Layermask32 mask = Layermask32::ALL;

		// the damage output of this bullet
		float damage = 1.0f;

		// returns the last character that this bullet collided with
		EntityID GetLastCollision() const { return m_lastCollision; }

	private:
		EntityID m_lastCollision = nullptr;
	public:
		void __SetCol(EntityID id) { m_lastCollision = id; }
	};

	struct CharacterBody final {

		// is physics applied
		bool isSimulated = true;

		// velocity of the character
		vec2 velocity = vec2(0.0f);

		// radius of the character
		// physics does not apply if the radius is < 0.0001
		float radius = 0.5f;

		// what layers does this character collide with
		Layermask32 mask = Layermask32::ALL;

		// iterator used for iterating the collision list
		using const_iterator = vector<BCollisionInfo>::const_iterator;

		// iterator pointing to the begining of the collision list
		const_iterator begin() const { return m_collisions.begin(); }

		// iterator pointing to the end of the collision list
		const_iterator end() const { return m_collisions.end(); }

		// returns the number of collisions 
		size_t Count() const { return m_collisions.size(); }

		// returns collision at index
		const BCollisionInfo& operator[](const size_t index) const { return m_collisions[index]; }

	private:
		vector<BCollisionInfo> m_collisions;
	public:
		vector<BCollisionInfo>& __GetColl() ALC_INTERNAL { return m_collisions; }
	};

}

#endif // !ALC_BULLETS_BULLETPHYSICS_HPP