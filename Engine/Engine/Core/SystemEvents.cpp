#include "SystemEvents.hpp"
#include <SDL_events.h>
#include "Timer.hpp"
#include "../Keyboard.hpp"
#include "../Mouse.hpp"
#include "../Game.hpp"

SystemEvents::SystemEvents(Game * game_) : game(game_) { }

SystemEvents::~SystemEvents() { }

void SystemEvents::PollEvents() {
	Keyboard::ClearChangedBits();

	static SDL_Event ev;
	while (SDL_PollEvent(&ev)) {

		switch (ev.type) {
			case SDL_QUIT:
				if (game) game->Quit();
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				KeyboardEvent(ev.key);
				break;

			default: break;
		}
	}

	/// other data
	UpdateKeyMods();
	UpdateMouse();
}

void SystemEvents::KeyboardEvent(SDL_KeyboardEvent& ke) {
	// no need to check the key if it's repeating
	if (ke.repeat) return;

	// letter on the keyboard
	if (ke.keysym.sym > 96 && ke.keysym.sym < 123)
		Keyboard::SetKey(static_cast<KeyCode>(ke.keysym.sym - 97), ke.state);
	// number on keyboard
	else if (ke.keysym.sym > 47 && ke.keysym.sym < 58)
		Keyboard::SetKey(static_cast<KeyCode>(ke.keysym.sym - 48), ke.state);
	// other
	else {
		switch (ke.keysym.scancode) {
			case SDL_SCANCODE_TAB:       Keyboard::SetKey(KeyCode::Tab, ke.state);        break;
			case SDL_SCANCODE_RETURN:    Keyboard::SetKey(KeyCode::Return, ke.state);     break;
			case SDL_SCANCODE_BACKSPACE: Keyboard::SetKey(KeyCode::Backspace, ke.state);  break;
			case SDL_SCANCODE_SPACE:     Keyboard::SetKey(KeyCode::Space, ke.state);      break;
			case SDL_SCANCODE_ESCAPE:    Keyboard::SetKey(KeyCode::Escape, ke.state);     break;
			case SDL_SCANCODE_UP:        Keyboard::SetKey(KeyCode::ArrowUp, ke.state);    break;
			case SDL_SCANCODE_DOWN:      Keyboard::SetKey(KeyCode::ArrowDown, ke.state);  break;
			case SDL_SCANCODE_LEFT:      Keyboard::SetKey(KeyCode::ArrowLeft, ke.state);  break;
			case SDL_SCANCODE_RIGHT:     Keyboard::SetKey(KeyCode::ArrowRight, ke.state); break;
			default: break;
		}
	}
}

void SystemEvents::UpdateKeyMods() {

	static SDL_Keymod m = SDL_GetModState();

	Keyboard::SetKey(KeyCode::LeftShift, m & KMOD_LSHIFT);
	Keyboard::SetKey(KeyCode::LeftCtrl, m & KMOD_LCTRL);
	Keyboard::SetKey(KeyCode::LeftAlt, m & KMOD_LALT);
	Keyboard::SetKey(KeyCode::RightShift, m & KMOD_RSHIFT);
	Keyboard::SetKey(KeyCode::RightCtrl, m & KMOD_RCTRL);
	Keyboard::SetKey(KeyCode::RightAlt, m & KMOD_RALT);

}

void SystemEvents::UpdateMouse() {
	mouseState = SDL_GetMouseState(&mousepos.x, &mousepos.y);

	Mouse::SetNewPosition(mousepos);
	Mouse::SetButtons(
		mouseState & SDL_BUTTON(SDL_BUTTON_LEFT),
		mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT),
		mouseState & SDL_BUTTON(SDL_BUTTON_MIDDLE)
	);
}



//void SystemEvents::PollEvents() {
//	Keyboard::ClearChangedBits();
//
//	static SDL_Event ev;
//	while (SDL_PollEvent(&ev)) {
//
//		switch (ev.type) {
//			case SDL_QUIT:
//				Engine::Get()->Quit();
//				break;
//			case SDL_WINDOWEVENT:
//				Engine::Get()->GetTime()->UnfreezeTime(ev.window.timestamp);
//				break;
//			case SDL_KEYDOWN:
//			case SDL_KEYUP:
//				KeyboardEvent(ev.key);
//				break;
//
//			default: break;
//		}
//	}
//
//	/// other data
//	UpdateKeyMods();
//	UpdateMouse();
//
//}

//void SystemEvents::KeyboardEvent(SDL_KeyboardEvent ke) {
//	// no need to check the key if it's repeating
//	if (ke.repeat) return;
//
//	// letter on the keyboard
//	if (ke.keysym.sym > 96 && ke.keysym.sym < 123)
//		Keyboard::SetKey(static_cast<KeyCode>(ke.keysym.sym - 97), ke.state);
//	// number on keyboard
//	else if (ke.keysym.sym > 47 && ke.keysym.sym < 58)
//		Keyboard::SetKey(static_cast<KeyCode>(ke.keysym.sym - 48), ke.state);
//	// other
//	else {
//		switch (ke.keysym.scancode) {
//			case SDL_SCANCODE_TAB:       Keyboard::SetKey(KeyCode::Tab, ke.state);        break;
//			case SDL_SCANCODE_RETURN:    Keyboard::SetKey(KeyCode::Return, ke.state);     break;
//			case SDL_SCANCODE_BACKSPACE: Keyboard::SetKey(KeyCode::Backspace, ke.state);  break;
//			case SDL_SCANCODE_SPACE:     Keyboard::SetKey(KeyCode::Space, ke.state);      break;
//			case SDL_SCANCODE_ESCAPE:    Keyboard::SetKey(KeyCode::Escape, ke.state);     break;
//			case SDL_SCANCODE_UP:        Keyboard::SetKey(KeyCode::ArrowUp, ke.state);    break;
//			case SDL_SCANCODE_DOWN:      Keyboard::SetKey(KeyCode::ArrowDown, ke.state);  break;
//			case SDL_SCANCODE_LEFT:      Keyboard::SetKey(KeyCode::ArrowLeft, ke.state);  break;
//			case SDL_SCANCODE_RIGHT:     Keyboard::SetKey(KeyCode::ArrowRight, ke.state); break;
//			default: break;
//		}
//	}
//}

//void SystemEvents::UpdateKeyMods() {
//
//	static SDL_Keymod m = SDL_GetModState();
//
//	Keyboard::SetKey(KeyCode::LeftShift, m & KMOD_LSHIFT);
//	Keyboard::SetKey(KeyCode::LeftCtrl, m & KMOD_LCTRL);
//	Keyboard::SetKey(KeyCode::LeftAlt, m & KMOD_LALT);
//	Keyboard::SetKey(KeyCode::RightShift, m & KMOD_RSHIFT);
//	Keyboard::SetKey(KeyCode::RightCtrl, m & KMOD_RCTRL);
//	Keyboard::SetKey(KeyCode::RightAlt, m & KMOD_RALT);
//
//}

//void SystemEvents::UpdateMouse() {
//	static int x = 0, y = 0;
//	static uint state = 0;
//	state = SDL_GetMouseState(&x, &y);
//
//	Mouse::SetNewPosition(x, y);
//	Mouse::SetButtons(
//		state & SDL_BUTTON(SDL_BUTTON_LEFT),
//		state & SDL_BUTTON(SDL_BUTTON_RIGHT),
//		state & SDL_BUTTON(SDL_BUTTON_MIDDLE)
//	);
//
//}
