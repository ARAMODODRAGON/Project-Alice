#include "BTA.hpp"

namespace BTA {

	Result MoveTo(
		ALC::vec2* outVelocity,
		ALC::vec2& currentpos,
		const ALC::vec2& destination,
		const float acceleration,
		const float maxSpeed,
		ALC::Timestep ts
	) {
		// confirm that velocity is not nullptr
		if (outVelocity == nullptr)
			return Result::Error;

		const float currentSpeed = glm::length(*outVelocity);

		// check if arrived
		if (ALC::NearlyEqual(currentpos, destination, currentSpeed * ts + 1.5f)) {
			// stop and move to the *exact* position
			*outVelocity = ALC::vec2(0.0f);
			currentpos = destination;
			return Result::Arrived;
		}
		// has not arrived continue moving

		float velocity = glm::length(*outVelocity);

		const ALC::vec2 veldir = glm::normalize(destination - currentpos);
		const float distRemaining = glm::length(currentpos - destination);
		const float timeToSlowdown = (-maxSpeed / acceleration);
		const float distToSlowdown = glm::abs(maxSpeed * timeToSlowdown + 0.5f * acceleration * timeToSlowdown * timeToSlowdown);

		// check if we should slow down or not

		// slow down
		if (distRemaining < distToSlowdown) {
			velocity = maxSpeed * (distRemaining / distToSlowdown);
		}
		// speed up
		else {
			velocity += acceleration * ts;
			// cap speed
			if (velocity > (maxSpeed * maxSpeed)) velocity = maxSpeed;
		}

		*outVelocity = veldir * velocity;
		return Result::Moving;
	}

}