#ifndef _CORE_TIMER_HPP
#define _CORE_TIMER_HPP

// runs on windows only
#if defined(_WIN32)
#include <Windows.h>
#include <profileapi.h>
#endif
// defined by windows for some reason...
#undef GetObject
#undef DrawText

class Timer {

	LARGE_INTEGER counts_per_frame;
	LARGE_INTEGER counts_per_second;
	LARGE_INTEGER lastCounts;
	LARGE_INTEGER currentCounts;

	unsigned int fps;

public:

	Timer();
	~Timer();

	// getter & setter
	unsigned int GetFPS() const { return fps; }
	void SetFPS(unsigned int fps_) { fps = fps_; counts_per_frame.QuadPart = counts_per_second.QuadPart / fps; }

	// events
	float RemainingTimeUntilNextFrame();
	void WaitForEndOfFrame();

};

#endif // !_CORE_TIMER_HPP