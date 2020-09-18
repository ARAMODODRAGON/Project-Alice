#include "Timer.hpp"
#include "Debugger.hpp"

Timer::Timer()
	: fps(60), counts_per_frame { 0 }, counts_per_second { 0 }, lastCounts { 0 }, currentCounts { 0 } {
	// get the count frequency
	if (!QueryPerformanceFrequency(&counts_per_second)) {
		DEBUG_ERROR("System does not support a high-resolution performance counter");
	} else {
		counts_per_frame.QuadPart = counts_per_second.QuadPart / fps;
	}

}

Timer::~Timer() { }

float Timer::RemainingTimeUntilNextFrame() {
	// get remaining time
	LARGE_INTEGER curCount = { 0 };
	QueryPerformanceCounter(&curCount);
	return float(counts_per_second.QuadPart) / float(curCount.QuadPart - curCount.QuadPart);
}

void Timer::WaitForEndOfFrame() {
	// get remaining time
	lastCounts = currentCounts;
	QueryPerformanceCounter(&currentCounts);

	// wait for end of frame
	while ((currentCounts.QuadPart - lastCounts.QuadPart) < counts_per_frame.QuadPart)
		QueryPerformanceCounter(&currentCounts);
}
