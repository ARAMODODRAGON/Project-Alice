#ifndef ALC_PHYSICS_RIGIDBODY2D_HPP
#define ALC_PHYSICS_RIGIDBODY2D_HPP
#include "CollisionInfo.hpp"
#include "Transform2D.hpp"
#include "../Layermask.hpp"

namespace ALC {

	enum class ShapeType : uint8 {
		None, Circle
	};

	struct CircleShape final {
		// the enum shape type of this shape
		static constexpr ShapeType SHAPE_TYPE = ShapeType::Circle;

		// offset from the Transform2D
		vec2 offset = vec2(0.0f);

		// the radius of the circle
		float radius = 0.5f;

	};

	struct Rigidbody2D final {
		// used to iterate over the collisions
		using const_iterator = list<CollisionInfo>::const_iterator;

		// enables physics 
		bool isSimulated = true;

		// current velocity
		vec2 velocity = vec2(0.0f);

		// current acceleration
		vec2 acceleration = vec2(0.0f);

		// the collision mask
		Layermask32 collisionMask = Layermask32::NONE;

		// the trigger mask
		Layermask32 triggerMask = Layermask32::NONE;

		// prioritize collisions or triggers
		// if true then it will always collide as long as
		// collision mask matches other collision mask
		// 'true' always has priority over 'false'
		bool preferCollisions = false;

		// sets the shape of the rigidbody
		template<typename Shape>
		Shape& SetShape();

		// returns shape of type
		template<typename Shape>
		Shape& GetShape();

		// returns the type of shape
		ShapeType GetShapeType() const { return m_shapetype; }

		// returns true if this is the current shape
		template<typename Shape>
		bool HasShape() const { return Shape::SHAPE_TYPE == m_shapetype; }

		// returns the number of collisions
		uint32 GetCollisionCount() const { return m_collisions.size(); }

		// returns an iterator pointing to the begining of the collisions
		const_iterator begin() const { return m_collisions.begin(); }

		// returns an iterator pointing to the end of the collision
		const_iterator end() const { return m_collisions.end(); }

		Rigidbody2D() : m_shape{} { }
	private:

		ShapeType m_shapetype = ShapeType::None;
		union {
			CircleShape circle;
		} m_shape;
		list<CollisionInfo> m_collisions;

	public:
		list<CollisionInfo>& __GetCollInfo() { return m_collisions; }
	};

	template<typename Shape>
	inline Shape& Rigidbody2D::SetShape() {
		// return new shape
		if (Shape::SHAPE_TYPE != m_shapetype) {
			m_shapetype = Shape::SHAPE_TYPE;
			switch (Shape::SHAPE_TYPE) {
				case ShapeType::Circle: return m_shape.circle = CircleShape();
				default: break;
			}
		}
		// return old shape
		else {
			switch (Shape::SHAPE_TYPE) {
				case ShapeType::Circle: return m_shape.circle;
				default: break;
			}
		}
		ALC_ASSERT(false, "Shape type is not supported");
	}

	template<typename Shape>
	inline Shape& Rigidbody2D::GetShape() {
		ALC_ASSERT(Shape::SHAPE_TYPE == m_shapetype, "shape type must match");
		switch (Shape::SHAPE_TYPE) {
			case ShapeType::Circle: return m_shape.circle;
			default: break;
		}
		ALC_ASSERT(false, "Shape type is not supported");
	}

	inline CollisionInfo::CollisionInfo(
		Layermask32 collisionMask,
		Layermask32 triggerMask,
		EntityID other,
		CollisionState state)
		: m_collisionMask(collisionMask)
		, m_triggerMask(triggerMask)
		, m_other(other)
		, m_state(state) { }

}

#endif // !ALC_PHYSICS_RIGIDBODY2D_HPP