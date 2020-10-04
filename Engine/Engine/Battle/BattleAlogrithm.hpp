#ifndef BATTLE_ALOGRITHM_HPP
#define BATTLE_ALOGRITHM_HPP
#include <glm/glm.hpp>
#include <iostream>

namespace bta {

	glm::vec2 MoveTo(glm::vec2 startPos_, glm::vec2 destination_, float maxAcceleration_,float maxSpeed_);

}

#endif // !BATTLE_ALOGRITHM_HPP


