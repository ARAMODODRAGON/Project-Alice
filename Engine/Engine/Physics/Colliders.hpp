#ifndef _PHYSICS_COLLIDERS_HPP
#define _PHYSICS_COLLIDERS_HPP
#include "../General/Types.hpp"
#include "../General/Serialization.hpp"
#include <glm/gtx/norm.hpp>
#include "ColType.hpp"
#include <functional>

typedef unsigned int uint;

struct LayerMask {

	// constructors / operators
	LayerMask() : mask(0) { }
	LayerMask(const uint mask_) : mask(mask_) { }
	LayerMask& operator=(const uint mask_) { mask = mask_; }

	// operators

	// cast to unsigned int
	explicit operator uint() const { return mask; }

	// checks if the masks match exactly
	bool operator==(const LayerMask& other) const { return mask == other.mask; }

	// checks if the masks dont match exactly
	bool operator!=(const LayerMask& other) const { return mask != other.mask; }

	// returns true if *any* layers match
	bool operator&&(const LayerMask& other) const { return mask & other.mask; }

	// returns a layermask with only the matching layers
	LayerMask operator&(const LayerMask& other) const { return LayerMask(mask & other.mask); }
	LayerMask& operator&=(const LayerMask& other) {
		mask &= other.mask;
		return *this;
	}

	// used to check if any layers are set
	operator bool() const { return mask != 0; }

	// creates a new layermask with the layers of the two old ones
	LayerMask operator|(const LayerMask& other) const { return LayerMask(mask | other.mask); }
	LayerMask& operator|=(const LayerMask& other) {
		mask |= other.mask;
		return *this;
	}

	// returns a reversed layermask
	LayerMask operator~() const { return LayerMask(~mask); }

	// function to set/unset layers
	void SetLayer(const uint layer, bool value) {
		if (value)	mask |= (1 << layer);  // set true
		else		mask &= ~(1 << layer); // set false
	}

	// function to get layer
	bool GetLayer(const uint layer) const {
		return mask & (1 << layer);
	}

private:

	// 32 layers of bools essentially
	uint mask;

};

struct BoundingCircle {

	vec2 position;
	float radius;

	BoundingCircle() : position(0.0f), radius(0.0f) { }
	BoundingCircle(const vec2& position_, const float& radius_)
		: position(position_), radius(radius_) { }

	static bool Intersects(const BoundingCircle& b0, const BoundingCircle& b1) {
		// minimum distance^2 for these two BCs to be overlapping
		float minDistanceSqr = b0.radius + b1.radius;
		minDistanceSqr *= minDistanceSqr; // square it

		// compare to the actual distance^2
		if (glm::length2(b0.position - b1.position) < minDistanceSqr)
			return true;

		// not overlapping
		return false;
	}

};

class Collider;

class CollisionData {
	// the other collider in this collision
	Collider* otherCollider;
	// the collider on the object that collided
	Collider* thisCollider;
};

// does not inherit from component since some colliders may not be components (ie. TilemapCollider)
class Collider {
	friend class PhysicsScene;

	// data
	bool isSimulated;
	vec2 position;
	const ColType coltype;
	BoundingCircle bounds;
	LayerMask collisionMask;
	LayerMask triggerMask;

	// collisions
	forward_list<CollisionData> collisions;

public:

	Collider(ColType coltype_) : position(0.0f), coltype(coltype_) { }
	virtual ~Collider() = 0; // abstract

	// getters and setters
	bool GetIsSimulated() const { return isSimulated; }
	void SetIsSimulated(const bool isSimulated_) { isSimulated = isSimulated_; }
	vec2 GetPosition() const { return position; }
	void SetPosition(const vec2& position_) { position = position_; }
	ColType GetColType() const { return coltype; }
	virtual BoundingCircle GetBounds() const = 0;

	RTTR_ENABLE() RTTR_REGISTRATION_FRIEND
};

#endif // !_PHYSICS_COLLIDERS_HPP