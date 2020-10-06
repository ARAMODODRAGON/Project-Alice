#include "BattleAlogrithm.hpp"


glm::vec2 bta::MoveTo(glm::vec2 ObjStartPos_, glm::vec2 destination_, float maxAcceleration_,float maxSpeed_)
{
    glm::vec2 direction;
    glm::vec2 targetVelocity;
    glm::vec2 velocity;

    float distance = 0;
    float slowRadius = 4.0f;
    float targetRadius = 1.0f;
    float timeToTarget = .1f;
    float targetSpeed;


    direction = destination_ - ObjStartPos_;

    distance = glm::length(direction);

    velocity = glm::normalize(direction);
    //velocity *= maxAcceleration_;
 

    if (distance < targetRadius) {
        distance = 0;
        maxAcceleration_ = 0.0f;
    }

    if (distance > slowRadius) {
        targetSpeed = maxSpeed_;
        velocity *= targetSpeed;
    }

    else {
        targetSpeed = maxSpeed_ * (distance / slowRadius);
        targetVelocity = direction;
        targetVelocity = glm::normalize(targetVelocity);
        targetVelocity *= targetSpeed;

        velocity = targetVelocity - direction;
        velocity /= timeToTarget;
    }

    if (glm::length(velocity) > maxAcceleration_) {
        velocity = glm::normalize(velocity);
        velocity *= maxAcceleration_;

    }


    return velocity;

}


