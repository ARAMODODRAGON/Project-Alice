#ifndef _RENDERING_RECT_HPP
#define _RENDERING_RECT_HPP
#include "../General/Math.hpp"

struct Rect {
	vec2 min, max;

	Rect() { }

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

};

#endif // !_RENDERING_RECT_HPP