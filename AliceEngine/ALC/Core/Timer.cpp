#include "Timer.hpp"
#include "Debugger.hpp"

namespace ALC {

	Timer::Timer() : targetFPS(0), lastDelta(0.0) {
		SetTargetFPS(60);
		lastTime = currentTime = steady_clock::now();
	}

	Timer::~Timer() { }

	void Timer::BeginFrame() {
		lastDelta = (currentTime - lastTime).count();
		// save the previous time point
		lastTime = currentTime;
	}

	void Timer::WaitForEndOfFrame() {
		// get new time
		time_point t = steady_clock::now();

		// loop as long as this frame is not done yet
		while ((t - lastTime).count() < secondsPerFrame) {
			t = steady_clock::now();
		}

		currentTime = t;
	}

	bool Timer::CheckIfFrameComplete() {
		// check if this frame is done yet
		if ((steady_clock::now() - lastTime).count() > secondsPerFrame) {
			// assume that the caller wants to update time
			currentTime = steady_clock::now();
			return true;
		}
		return false;
	}

	double Timer::GetFPS() const {
		return 1.0 / lastDelta;
	}

	double Timer::GetDelta() const {
		return lastDelta;
	}

}