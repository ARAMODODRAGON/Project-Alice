#ifndef _CORE_TIMER_HPP
#define _CORE_TIMER_HPP

#include <chrono>

class Timer {
	using steady_clock = std::chrono::steady_clock;
	using duration = std::chrono::duration<double>;
	using time_point = std::chrono::time_point<steady_clock, duration>;

	time_point lastTime;
	time_point currentTime;

	unsigned int targetFPS;
	double secondsPerFrame;

public:

	Timer();
	~Timer();

	// functions
	void WaitForEndOfFrame();
	bool CheckIfFrameComplete();

	// getters and setters
	unsigned int GetTargetFPS() const { return targetFPS; }
	void SetTargetFPS(const unsigned int& targetFPS_) {
		targetFPS = targetFPS_;
		secondsPerFrame = 1.0 / static_cast<double>(targetFPS);
	}
	double GetFPS() const;
	double GetDelta() const;

};

#endif // !_CORE_TIMER_HPP