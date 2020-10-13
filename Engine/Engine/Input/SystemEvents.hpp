#ifndef __EVENTS_SYSTEM_EVENTS_HPP
#define __EVENTS_SYSTEM_EVENTS_HPP
#include "../General/Macros.hpp"
#include "../General/Types.hpp"

class Game;

class SystemEvents {
	PRIVATE_SINGLETON(SystemEvents);

	Game* game;
	ivec2 mousepos;
	int32 mouseState;

	SystemEvents();
	~SystemEvents();

public:

	// events
	static void Init(Game* game_);
	static void PollEvents();
	static void Exit();

private:

	void KeyboardEvent(struct SDL_KeyboardEvent& ke);
	void UpdateKeyMods();
	void UpdateMouse();

};

#endif // !__EVENTS_SYSTEM_EVENTS_HPP