#ifndef ALC_RECT_HPP
#define ALC_RECT_HPP
#include <glm\glm.hpp>

namespace ALC {

	using glm::vec2;

	struct rect {
		// data
		union {
			struct { vec2 min, max; };
			struct { float left, bottom, right, top; };
		};

		rect() : min(0.0f), max(0.0f) { }
		rect(const float halfSize_) : min(-halfSize_), max(halfSize_) { }
		rect(const vec2& min_, const vec2& max_)
			: min(min_), max(max_) { }
		rect(const float left_, const float bottom_, const float right_, const float top_)
			: min(left_, bottom_), max(right_, top_) { }

		static bool Intersects(const rect& r0, const rect& r1) {
			if (r0.min.x > r1.max.x || r0.max.x < r1.min.x) return false;
			if (r0.min.y > r1.max.y || r0.max.y < r1.min.y) return false;
			return true;
		}

	};

}

#endif // !ALC_RECT_HPP