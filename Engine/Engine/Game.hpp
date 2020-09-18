#ifndef _GAME_HPP
#define _GAME_HPP
#include "Core/Window.hpp"
#include "Core/Timer.hpp"
#include "Core/Debugger.hpp"
#include "General/Serialization.hpp"
#include "Core/Level.hpp"
#include "Rendering/ContentHandler.hpp"
#include "Rendering/ContentHandler.hpp"

class Game {

	// the main instance of the game
	static Game* _instance;

	Window* window;
	Timer* timer;

	bool isRunning;
	bool shouldQuit;

public:

	Game();
	virtual ~Game() = 0;

	// functions
	static Game* Get() { return _instance; }
	template<class T> static T* Get() { return dynamic_cast<T*>(_instance); }
	void Run();
	void Quit() { shouldQuit = true; }

	// getters
	Window* GetWindow() const { return window; }
	Timer* GetTimer() const { return timer; }

protected:

	// events
	virtual bool Init();
	virtual void Update();
	virtual void Draw();
	virtual bool Exit();
	void PollEvents();

};

#endif // !_GAME_HPP