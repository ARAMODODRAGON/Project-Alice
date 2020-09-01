#ifndef __MATH_BOUNDS_HPP
#define __MATH_BOUNDS_HPP
#include <glm/vec2.hpp>
using glm::vec2;

struct Bounds {
	vec2 min, max;

	Bounds() 
		: min(0.0f), max(0.0f) { }
	Bounds(const vec2& min_, const vec2& max_) 
		: min(min_), max(max_) { }

	/// member functions

	bool Contains(const Bounds& other) const {
		// if not inside in each axis
		if (!(other.min.x >= min.x && other.max.x <= max.x)) return false;
		if (!(other.min.y >= min.y && other.max.y <= max.y)) return false;

		// it does contain it if none of the previous evaluated to true
		return true;
	}

	bool Contains(const vec2& point) const {
		// if not inside in each axis
		if (!(point.x >= min.x && point.x <= max.x)) return false;
		if (!(point.y >= min.y && point.y <= max.y)) return false;

		// it does contain it if none of the previous evaluated to true
		return true;
	}

	/// static functions

	static bool Intersects(const Bounds& b0, const Bounds& b1) {
		// check if they do not overlap
		if (b0.min.x > b1.max.x || b0.max.x < b1.min.x) return false;
		if (b0.min.y > b1.max.y || b0.max.y < b1.min.y) return false;
		
		// they do overlap if none of the previous evaluated to true
		return true;
	}

	static Bounds MakeFrom(const Bounds& b0, const Bounds& b1) {
		Bounds newBounds;

		newBounds.min.x = (b0.min.x < b1.min.x ? b0.min.x : b1.min.x);
		newBounds.min.y = (b0.min.y < b1.min.y ? b0.min.y : b1.min.y);
		newBounds.max.x = (b0.max.x > b1.max.x ? b0.max.x : b1.max.x);
		newBounds.max.y = (b0.max.y > b1.max.y ? b0.max.y : b1.max.y);

		return newBounds;
	}

};

#endif // !__MATH_BOUNDS_HPP