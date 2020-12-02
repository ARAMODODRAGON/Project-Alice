#ifndef BATTLE_ALOGRITHM_HPP
#define BATTLE_ALOGRITHM_HPP

#include <glm/glm.hpp>
#include <iostream>
#include <queue>
#include "../General/Math.hpp"
#include "../Core/Debugger.hpp"
#include <time.h>
#include <glm/gtc/random.hpp>
#include "TileMap/TileMap.hpp"

enum class BTAResult {
	Success,
	Error,
	Moving,
	Arrived
};

namespace bta {

	BTAResult MoveTo(vec2* velocity, const vec2& startPos_, const vec2& destination_, float maxAcceleration_, float maxSpeed_);
	BTAResult FleeFrom(vec2* velocity, const vec2& startPos_, const vec2& destination_, float maxAcceleration_, float maxSpeed_);
	bool PathTo(std::pair<int32, int32> startPos,std::pair<int32,int32> destination,int32 arr[ROW][COL]);

}			

#endif // !BATTLE_ALOGRITHM_HPP


