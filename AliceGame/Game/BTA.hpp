#ifndef ALICE_BTA_HPP
#define ALICE_BTA_HPP
#include <ALC/General.hpp>
#include <ALC/Core/Timer.hpp>

namespace BTA {

	enum class Result {
		Success,
		Error,
		Moving,
		Arrived
	};

	// returns Result::Arrived if currentpos == destination
	// returns Result::Error if outVelocity is null
	// otherwise returns Result::Moving
	Result MoveTo(
		ALC::vec2* outVelocity, 
		ALC::vec2& currentpos, 
		const ALC::vec2& destination, 
		const float acceleration, 
		const float maxSpeed,
		ALC::Timestep ts
	);

}

#endif // !ALICE_BTA_HPP