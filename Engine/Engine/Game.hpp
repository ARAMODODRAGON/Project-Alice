#ifndef _GAME_HPP
#define _GAME_HPP
#include "Core/Window.hpp"
#include "Core/Timer.hpp"
#include "Core/Debugger.hpp"
#include "Core/SystemEvents.hpp"
#include "General/Serialization.hpp"

class Game {

	Window* window;
	Timer* timer;
	SystemEvents* sysEvents;

	bool isRunning;
	bool shouldQuit;

public:

	Game();
	virtual ~Game() = 0;

	// functions
	void Run();
	void Quit() { shouldQuit = true; }
	
	// getters
	Window* GetWindow() const { return window; }
	Timer* GetTimer() const { return timer; }
	SystemEvents* GetSystemEvents() const { return sysEvents; }

protected:

	// inherited events
	virtual bool Start();
	virtual void Update();
	virtual void Draw();
	virtual bool Exit();

};

#endif // !_GAME_HPP