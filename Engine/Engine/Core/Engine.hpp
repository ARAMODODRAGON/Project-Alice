#ifndef _CORE_ENGINE_HPP
#define _CORE_ENGINE_HPP
#include "../GeneralMacros.hpp"
#include "../GeneralTypes.hpp"
#include <Windows.h>
#include <profileapi.h>

class Engine {
	SINGLETON(Engine);

	Engine();
	~Engine();

	// state and time
	bool isRunning : 1;
	bool shouldQuit : 1;
	LARGE_INTEGER counts_per_frame;
	LARGE_INTEGER counts_per_second;
	LARGE_INTEGER lastCounts;
	LARGE_INTEGER currentCounts;

	// window & opengl
	struct SDL_Window* window;
	void* glContext;
	int screenSizeX, screenSizeY;

public:

	// functions
	void Run();
	void Quit() { shouldQuit = true; }

private:

	// events
	void Start();
	void PollEvents();
	void Update();
	void Draw();
	void WaitForEndOfFrame();
	void Exit();

};

#endif // !_CORE_ENGINE_HPP