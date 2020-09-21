#ifndef _PHYSICS_COLLIDERS_HPP
#define _PHYSICS_COLLIDERS_HPP
#include "../Objects/Object.hpp"

// does not inherit from component since some colliders may not be components
class Collider {

	vec2 offset;

public:

	Collider() : offset(0.0f) { }
	virtual ~Collider() = 0;

	// getters and setters
	vec2 GetOffset() const { return offset; }
	void SetOffset(const vec2& offset_) { offset = offset_; }

};

class CircleCollider : Component {


public:

};

class BoxCollider : Component {


public:

};

#endif // !_PHYSICS_COLLIDERS_HPP