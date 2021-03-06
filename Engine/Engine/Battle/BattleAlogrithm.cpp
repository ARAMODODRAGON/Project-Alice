#include "BattleAlogrithm.hpp"

namespace bta {

	BTAResult MoveTo(vec2* velocity, const vec2& startPos_, const vec2& destination_, float maxAcceleration_, float maxSpeed_) {
		// check to see if velocity exist 
		if (velocity == nullptr) {
			DEBUG_ERROR("Velocity was nullptr, could not calculate new velocity");
			return BTAResult::Error;
		}

		
		glm::vec2 direction(0.0f); // points from the current position to the destination
		glm::vec2 targetVelocity(0.0f);

		float distance = 0;
		float slowRadius = 4.0f;
		float targetRadius = 1.0f;
		float timeToTarget = .1f;
		float targetSpeed;


		direction = destination_ - startPos_;

		distance = glm::length(direction);
		*velocity = glm::normalize(direction);

		*velocity *= maxAcceleration_;


		if (NearlyEqual(distance, targetRadius,4.0f)) {	  // if object is within 4 units of target stop
			distance = 0;
			maxAcceleration_ = 0.0f;
			*velocity = vec2(0.0f);
			return BTAResult::Arrived; // object has arrived at its destination 
		}	   

		else if (distance > slowRadius) {  // if object distance is greater than the slow radius  move at max speed
			targetSpeed = maxSpeed_;
		}

		else {	 // if objects distance is less than the slow radius start to slow the object down 
			targetSpeed = maxSpeed_ * (distance / slowRadius);
			targetVelocity = direction;
			targetVelocity = glm::normalize(targetVelocity);
			targetVelocity *= targetSpeed;

			*velocity = targetVelocity - direction;
			*velocity /= timeToTarget ;

		}


		if (glm::length(*velocity) > maxAcceleration_) {
			*velocity = glm::normalize(*velocity);
			*velocity *= maxAcceleration_;

		}

		// this makes sure we dont overshoot by checking if our velocity is more than the remaining distance
		if (glm::length(*velocity) > distance) {
			*velocity = glm::normalize(direction) * distance;
		}
		//DEBUG_ERROR("distance is: " + VTOS(distance));
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
		glm::vec2 offset;
		direction = startPos_ - destination_;

		float targetRadius = 1.0f;

		if (NearlyZero(direction,targetRadius)) {
			//DEBUG_LOG("Checked");
			 
			//Get offset of	value from -0.4 to 0.5
			//maybe a bug if both x,y = 0
			offset.x = glm::linearRand(-0.5f, 0.5f); 
			offset.y = glm::linearRand(-0.5f, 0.5f);
			//DEBUG_LOG("Offset is: " + VTOS(offset));
			
			//inital offset to get object moving 
			*velocity += offset;
		}

		else {
			*velocity = glm::normalize(direction); // only normalize direction if its greater than 0
		}

		*velocity *= maxAcceleration_;

		if (glm::length(*velocity) > maxAcceleration_) {
			*velocity = glm::normalize(*velocity);
			*velocity *= maxAcceleration_;

		}
		return BTAResult::Moving;
	}

	bool PathTo(std::pair<int32, int32> startPos,std::pair<int32,int32> destination,int32 arr[ROW][COL])
	{
		int32 direction[4][2] = { {0, 1},
			                      {0,-1},
			                      {1, 0},
			                      {-1,0} };

		std::queue<std::pair<int32,int32>> tiles;
		auto& tileVector = TileMap::GetTileVector();
		tiles.push(startPos);

		while (!tiles.empty()) {

			std::pair<int32, int32> reached = tiles.front();
			tiles.pop();
			arr[reached.first][reached.second] = -1;

			if (reached.first > ROW && reached.second > COL) {
				return false;
			}

			if (reached == destination) {
				return true;
			}

			for (size_t i = 0; i < 4; ++i) {
				uint32 a = reached.first + direction[i][0];
				uint32 b = reached.second + direction[i][1];

				if (a < ROW && b < COL && arr[a][b] != -1 && a >= 0 && b >= 0) {
					tiles.push(std::make_pair(a, b));
				}
			}
		}

		return false;
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
