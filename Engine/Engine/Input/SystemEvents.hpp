#ifndef __EVENTS_SYSTEM_EVENTS_HPP
#define __EVENTS_SYSTEM_EVENTS_HPP
#include "../General/Macros.hpp"
#include "../General/Math.hpp"

class Game;

class SystemEvents {

	Game* game;
	ivec2 mousepos;
	unsigned int mouseState;

public:

	SystemEvents(Game* game_);
	~SystemEvents();

	void PollEvents();

private:

	void KeyboardEvent(struct SDL_KeyboardEvent& ke);
	void UpdateKeyMods();
	void UpdateMouse();

};

#endif // !__EVENTS_SYSTEM_EVENTS_HPP