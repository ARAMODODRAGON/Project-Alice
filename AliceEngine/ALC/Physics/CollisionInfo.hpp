/*
* MIT License
*
* Copyright (c) 2021 Domara Shlimon
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
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