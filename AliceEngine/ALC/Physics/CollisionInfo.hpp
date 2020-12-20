#ifndef ALC_PHYSICS_COLLISIONINFO_HPP
#define ALC_PHYSICS_COLLISIONINFO_HPP
#include "../Layermask.hpp"
#include "../Entities/EntityID.hpp"

namespace ALC {


	enum class CollisionState : uint8 {
		None, Begin, Stay, End
	};

	// contains data on a collision
	struct CollisionInfo final {

		// layers that match in the collision
		Layermask32 GetCollisionMask() const { return m_collisionMask; }

		// layers that match in the trigger
		Layermask32 GetTriggerMask() const { return m_triggerMask; }

		// other entity involved in collision
		EntityID GetOther() const { return m_other; }

		// get the current collision state
		CollisionState GetState() const { return m_state; }

		CollisionInfo() = default;
	private:

		Layermask32 m_collisionMask = Layermask32::NONE;
		Layermask32 m_triggerMask = Layermask32::NONE;
		EntityID m_other;
		CollisionState m_state = CollisionState::None;

	public:
		void __SetState(CollisionState state) { m_state = state; }
		CollisionInfo(Layermask32 collisionMask, Layermask32 triggerMask, EntityID other, CollisionState state) ALC_INTERNAL;
	};


}

#endif // !ALC_PHYSICS_COLLISIONINFO_HPP