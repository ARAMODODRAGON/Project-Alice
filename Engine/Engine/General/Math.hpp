#ifndef _MATH_HPP
#define _MATH_HPP

#include <glm/glm.hpp>
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::ivec2;
using glm::ivec3;
using glm::ivec4;
using glm::uvec2;
using glm::uvec3;
using glm::uvec4;
using glm::mat3;
using glm::mat4;

#include <glm/gtx/string_cast.hpp>
using glm::to_string;

struct Rect {
	union {
		struct { vec2 min, max; };
		struct { float left, bottom, right, top; };
	};

	Rect() : min(0.0f), max(0.0f) { }

	Rect(const vec2& min_, const vec2& max_)
		: min(min_), max(max_) { }

	bool Contains(const Rect& other) {
		// check if not inside
		if (min.x > other.min.x || max.x < other.max.x) return false;
		if (min.y > other.min.y || max.y < other.max.y) return false;
		// if all else fails then true
		return true;
	}

	static bool Intersects(const Rect& r0, const Rect& r1) {
		// check if they do not overlap
		if (r0.min.x > r1.max.x || r0.max.x < r1.min.x) return false;
		if (r0.min.y > r1.max.y || r0.max.y < r1.min.y) return false;
		// if all else fails then true
		return true;
	}

	vec2 Clamp(const vec2& pos) {
		vec2 ret = pos;
		if (ret.x > max.x) ret.x = max.x;
		if (ret.y > max.y) ret.y = max.y;
		if (ret.x < min.x) ret.x = min.x;
		if (ret.y < min.y) ret.y = min.y;
		return ret;
	}

};

#define DEFAULT_PERCISION 0.0001f
inline bool NearlyZero(const float& v, const float& percision = DEFAULT_PERCISION) {
	return (abs(v) < percision);
}
inline bool NearlyEqual(const float& v0, const float& v1, const float& percision = DEFAULT_PERCISION) {
	return (abs(v0 - v1) < percision);
}

inline bool NearlyZero(const vec2& v, const float& percision = DEFAULT_PERCISION) {
	return (abs(v.x) < percision)
		&& (abs(v.y) < percision);
}
inline bool NearlyEqual(const vec2& v0, const vec2& v1, const float& percision = DEFAULT_PERCISION) {
	return (abs(v0.x - v1.x) < percision)
		&& (abs(v0.y - v1.y) < percision);
}

inline bool NearlyZero(const vec3& v, const float& percision = DEFAULT_PERCISION) {
	return (abs(v.x) < percision)
		&& (abs(v.y) < percision)
		&& (abs(v.z) < percision);
}
inline bool NearlyEqual(const vec3& v0, const vec3& v1, const float& percision = DEFAULT_PERCISION) {
	return (abs(v0.x - v1.x) < percision)
		&& (abs(v0.y - v1.y) < percision)
		&& (abs(v0.z - v1.z) < percision);
}

inline bool NearlyZero(const vec4& v, const float& percision = DEFAULT_PERCISION) {
	return (abs(v.x) < percision)
		&& (abs(v.y) < percision)
		&& (abs(v.z) < percision)
		&& (abs(v.w) < percision);
}
inline bool NearlyEqual(const vec4& v0, const vec4& v1, const float& percision = DEFAULT_PERCISION) {
	return (abs(v0.x - v1.x) < percision)
		&& (abs(v0.y - v1.y) < percision)
		&& (abs(v0.z - v1.z) < percision)
		&& (abs(v0.w - v1.w) < percision);
}

#endif // !_MATH_HPP