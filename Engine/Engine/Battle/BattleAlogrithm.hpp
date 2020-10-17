#ifndef BATTLE_ALOGRITHM_HPP
#define BATTLE_ALOGRITHM_HPP

#include <glm/glm.hpp>
#include <iostream>
#include "../General/Math.hpp"
#include "../Core/Debugger.hpp"
#include <time.h>
#include <glm/gtc/random.hpp>

enum class BTAResult {
	Success,
	Error,
	Moving,
	Arrived
};

namespace bta {

	BTAResult MoveTo(vec2* velocity, const vec2& startPos_, const vec2& destination_, float maxAcceleration_, float maxSpeed_);
	BTAResult FleeFrom(vec2* velocity, const vec2& startPos_, const vec2& destination_, float maxAcceleration_, float maxSpeed_);

}

#endif // !BATTLE_ALOGRITHM_HPP


