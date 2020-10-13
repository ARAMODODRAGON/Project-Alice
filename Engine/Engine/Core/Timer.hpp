#ifndef _CORE_TIMER_HPP
#define _CORE_TIMER_HPP

#include <chrono>
#include "../General/Types.hpp"

class Timer {
	using steady_clock = std::chrono::steady_clock;
	using duration = std::chrono::duration<double>;
	using time_point = std::chrono::time_point<steady_clock, duration>;

	time_point lastTime;
	time_point currentTime;

	uint32 targetFPS;
	double secondsPerFrame;

public:

	Timer();
	~Timer();

	// functions
	void WaitForEndOfFrame();
	bool CheckIfFrameComplete();

	// getters and setters
	uint32 GetTargetFPS() const { return targetFPS; }
	void SetTargetFPS(const uint32& targetFPS_) {
		targetFPS = targetFPS_;
		secondsPerFrame = 1.0 / static_cast<double>(targetFPS);
	}
	double GetFPS() const;
	double GetDelta() const;

};

#endif // !_CORE_TIMER_HPP