#ifndef _GAME_HPP
#define _GAME_HPP
#include "Core/Window.hpp"
#include "Core/Timer.hpp"
#include "Core/Debugger.hpp"

class Game {

	Window* window;
	Timer* timer;

	bool isRunning;
	bool shouldQuit;

public:

	Game();
	virtual ~Game() = 0;

	// functions
	void Run();
	void Quit() { shouldQuit = true; }
	
	// getters
	Window* GetWindow() { return window; }
	Timer* GetTimer() { return timer; }

protected:

	// inherited events
	virtual bool Start();
	virtual void Update();
	virtual void Draw();
	virtual bool Exit();

private:

	// private events
	void PollEvents();

};

#endif // !_GAME_HPP