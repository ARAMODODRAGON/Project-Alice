#include "Timer.hpp"
#include "Debugger.hpp"

namespace ALC {

	Timer::Timer() : targetFPS(0) {
		SetTargetFPS(60);
		lastTime = currentTime = steady_clock::now();
	}

	Timer::~Timer() { }

	void Timer::BeginFrame() {
		// save the previous time point
		lastTime = currentTime;

		// get new time
		currentTime = steady_clock::now();
	}

	void Timer::WaitForEndOfFrame() {
		// get new time
		currentTime = steady_clock::now();

		//DEBUG_LOG("Current delta: " + VTOS((currentTime - lastTime).count()));

		// loop as long as this frame is not done yet
		while ((currentTime - lastTime).count() < secondsPerFrame) {
			currentTime = steady_clock::now();
		}
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
		return 1.0 / (currentTime - lastTime).count();
	}

	double Timer::GetDelta() const {
		return (currentTime - lastTime).count();
	}

}