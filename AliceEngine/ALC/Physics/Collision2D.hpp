#ifndef ALC_PHYSICS_COLLISION2D_HPP
#define ALC_PHYSICS_COLLISION2D_HPP
#include "Rigidbody2D.hpp"
#include <glm\gtx\norm.hpp>

namespace ALC {
	namespace Collision2D {

		bool CircleCircleTrig(Transform2D& t0, Rigidbody2D& r0, CircleShape& c0,
							  Transform2D& t1, Rigidbody2D& r1, CircleShape& c1);

	}
}

inline bool ALC::Collision2D::CircleCircleTrig(
	Transform2D& t0, Rigidbody2D& r0, CircleShape& c0, 
	Transform2D& t1, Rigidbody2D& r1, CircleShape& c1) {
	vec2 p0 = t0.position + c0.offset;
	vec2 p1 = t1.position + c1.offset;
	float dist = c0.radius + c1.radius;
	return glm::length2(p0 - p1) < (dist * dist) ;
}

#endif // !ALC_PHYSICS_COLLISION2D_HPP