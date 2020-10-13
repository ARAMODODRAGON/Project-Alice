#include "BattleAlogrithm.hpp"

namespace bta {

	BTAResult MoveTo(vec2* velocity, const vec2& startPos_, const vec2& destination_, float maxAcceleration_, float maxSpeed_) {
		// check 
		if (velocity == nullptr) {
			DEBUG_ERROR("Velocity was nullptr, could not calculate new velocity");
			return BTAResult::Arrived;
		}
		// points from the current position to the destination
		glm::vec2 direction;
		glm::vec2 targetVelocity;

		float distance = 0;
		float slowRadius = 4.0f;
		float targetRadius = 1.0f;
		float timeToTarget = 0.1f;
		float targetSpeed;


		direction = destination_ - startPos_;

		distance = glm::length(direction);
		*velocity = glm::normalize(direction);

		*velocity *= maxAcceleration_;


		if (NearlyZero(distance, targetRadius)) {
			distance = 0;
			maxAcceleration_ = 0.0f;
			*velocity = vec2(0.0f);
			return BTAResult::Arrived;
		}

		else if (distance > slowRadius) {
			targetSpeed = maxSpeed_;
			/*targetVelocity = direction;
			targetVelocity = glm::normalize(targetVelocity);
			targetVelocity *= targetSpeed;

			velocity = targetVelocity - direction;
			velocity /= timeToTarget;*/
		}

		else {
			targetSpeed = maxSpeed_ * (distance / slowRadius);
			targetVelocity = direction;
			targetVelocity = glm::normalize(targetVelocity);
			targetVelocity *= targetSpeed;

			*velocity = targetVelocity - direction;
			*velocity /= timeToTarget;

		}


		if (glm::length(*velocity) > maxAcceleration_) {
			*velocity = glm::normalize(*velocity);
			*velocity *= maxAcceleration_;

		}

		// this makes sure we dont overshoot by checking if our velocity is more than the remaining distance
		if (glm::length(*velocity) > distance) {
			*velocity = glm::normalize(direction) * distance;
		}

		return BTAResult::Moving;
	}

	BTAResult FleeFrom(vec2* velocity, const vec2& startPos_, const vec2& destination_, float maxAcceleration_, float maxSpeed_)
	{
		// check 
		if (velocity == nullptr) {
			DEBUG_ERROR("Velocity was nullptr, could not calculate new velocity");
			return BTAResult::Arrived;
		}
		// points from the current position to the destination
		glm::vec2 direction;
		glm::vec2 targetVelocity;

		float distance = 0;
		float slowRadius = 4.0f;
		float targetRadius = 1.0f;
		float timeToTarget = 0.1f;
		float targetSpeed;


		direction = startPos_ - destination_;

		distance = glm::length(direction);
		*velocity = glm::normalize(direction);

		*velocity *= maxAcceleration_;


		if (NearlyZero(distance, targetRadius)) {
			distance = 0;
			maxAcceleration_ = 0.0f;
			*velocity = vec2(0.0f);
			return BTAResult::Arrived;
		}

		else if (distance > slowRadius) {
			targetSpeed = maxSpeed_;
			/*targetVelocity = direction;
			targetVelocity = glm::normalize(targetVelocity);
			targetVelocity *= targetSpeed;

			velocity = targetVelocity - direction;
			velocity /= timeToTarget;*/
		}

		else {
			targetSpeed = maxSpeed_ * (distance / slowRadius);
			targetVelocity = direction;
			targetVelocity = glm::normalize(targetVelocity);
			targetVelocity *= targetSpeed;

			*velocity = targetVelocity - direction;
			*velocity /= timeToTarget;

		}


		if (glm::length(*velocity) > maxAcceleration_) {
			*velocity = glm::normalize(*velocity);
			*velocity *= maxAcceleration_;

		}

		// this makes sure we dont overshoot by checking if our velocity is more than the remaining distance
		if (glm::length(*velocity) > distance) {
			*velocity = glm::normalize(direction) * distance;
		}

		return BTAResult::Moving;
	}

}

//// check 
//if (velocity == nullptr) {
//	DEBUG_ERROR("Velocity was nullptr. Could not calculate new velocity");
//	return BTAResult::Error;
//}
//
//// points to the destination
//vec2 distanceDirection = destination - currentPosition;
//float distanceLength = glm::length(distanceDirection);
//
//// the current speed and direction
//float currentSpeed = glm::length(*velocity);
////vec2 currentDirection = glm::normalize(*velocity); // not required
//
//// we use max speed here so we dont predefine the slow radius
//// make sure that the radius is *not* bigger than half of the distance and reduce it otherwise
//float slowradius = maxSpeed;
//
//// acceleration that we calculate based on the max and min speed
//float acceleration = -currentSpeed * 0.5f * distanceLength;
//
//// if we are too far to slow down
//if (slowradius < distanceLength) {
//
//	// accelerate if we arnt at max speed
//	if (currentSpeed < maxSpeed) {
//		currentSpeed += acceleration;
//		// limit
//		if (currentSpeed > maxSpeed)
//			currentSpeed = maxSpeed;
//	}
//
//	// set the velocity based on the direction
//	*velocity = glm::normalize(distanceDirection) * currentSpeed;
//	return BTAResult::Moving;
//}
//
//// if we are close enough to slow down but not close enough to stop
//// we use minSpeed to calculate if we are in range because we dont want to go any slower than minSpeed
//else if (!NearlyEqual(currentPosition, destination, minSpeed)) {
//
//	// we want to slow down
//	currentSpeed -= acceleration;
//	// limit
//	if (currentSpeed < minSpeed)
//		currentSpeed = minSpeed;
//
//	// set the velocity based on direction
//	*velocity = glm::normalize(distanceDirection) * currentSpeed;
//	return BTAResult::Moving;
//}
//
//// if we are close enough to stop
//else {
//
//	// if we are exactly there then we stop
//	if (NearlyZero(distanceLength)) {
//		*velocity = vec2(0.0f);
//		return BTAResult::Arrived;
//	}
//
//	// we need to move one more time
//	else {
//		*velocity = distanceDirection;
//		return BTAResult::Moving;
//	}
//}
