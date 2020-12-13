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
		rect(const vec2& min_, const vec2& max_)
			: min(min_), max(max_) { }
		rect(const float left_, const float bottom_, const float right_, const float top_)
			: min(left_, bottom_), max(right_, top_) { }

	};

}

#endif // !ALC_RECT_HPP