#include "BattleAlogrithm.hpp"

glm::vec2 bta::MoveTo(glm::vec2 ObjStartPos_, glm::vec2 destination_, float maxAcceleration_,float maxSpeed_)
{
    glm::vec2 direction;
    float acceleration;

    direction = destination_ - ObjStartPos_;
    direction = glm::normalize(direction);
    direction *= maxAcceleration_;

    std::cout << direction.length() << std::endl;

    if (direction.length() > maxSpeed_) {
        direction = glm::normalize(direction);
        direction *= maxSpeed_;
        std::cout << "This Happened" << std::endl;
    }

    return direction;
}
