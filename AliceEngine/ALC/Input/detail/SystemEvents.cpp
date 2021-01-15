#include "SystemEvents.hpp"
#include <SDL_events.h>
#include "../Keyboard.hpp"
#include "../Mouse.hpp"
#include "../../SceneManager.hpp"

namespace ALC {
	namespace detail {

		void SystemEvents::PollEvents() {

			Keyboard::__ClearChangedBits();

			static SDL_Event ev;
			while (SDL_PollEvent(&ev)) {
				switch (ev.type) {
					case SDL_QUIT:
						SceneManager::Quit();
						break;
					case SDL_KEYDOWN:
					case SDL_KEYUP:
						__KeyboardEvent(ev.key);
						break;

					default: break;
				}
			}

			/// other data
			__UpdateKeyMods();
			__UpdateMouse();
		}

		void SystemEvents::__KeyboardEvent(SDL_KeyboardEvent& ke) {
			// no need to check the key if it's repeating
			if (ke.repeat) return;

			// letter on the keyboard
			if (ke.keysym.sym > 96 && ke.keysym.sym < 123)
				Keyboard::__SetKey(static_cast<KeyCode>(ke.keysym.sym - 97), ke.state);
			// number on keyboard
			else if (ke.keysym.sym > 47 && ke.keysym.sym < 58)
				Keyboard::__SetKey(static_cast<KeyCode>(ke.keysym.sym - 48), ke.state);
			// other
			else {
				switch (ke.keysym.scancode) {
					case SDL_SCANCODE_TAB:       Keyboard::__SetKey(KeyCode::Tab, ke.state);        break;
					case SDL_SCANCODE_RETURN:    Keyboard::__SetKey(KeyCode::Return, ke.state);     break;
					case SDL_SCANCODE_BACKSPACE: Keyboard::__SetKey(KeyCode::Backspace, ke.state);  break;
					case SDL_SCANCODE_SPACE:     Keyboard::__SetKey(KeyCode::Space, ke.state);      break;
					case SDL_SCANCODE_ESCAPE:    Keyboard::__SetKey(KeyCode::Escape, ke.state);     break;
					case SDL_SCANCODE_UP:        Keyboard::__SetKey(KeyCode::ArrowUp, ke.state);    break;
					case SDL_SCANCODE_DOWN:      Keyboard::__SetKey(KeyCode::ArrowDown, ke.state);  break;
					case SDL_SCANCODE_LEFT:      Keyboard::__SetKey(KeyCode::ArrowLeft, ke.state);  break;
					case SDL_SCANCODE_RIGHT:     Keyboard::__SetKey(KeyCode::ArrowRight, ke.state); break;
					default: break;
				}
			}
		}

		void SystemEvents::__UpdateKeyMods() {

			static SDL_Keymod m;
			m = SDL_GetModState();

			Keyboard::__SetKey(KeyCode::LeftShift, m & KMOD_LSHIFT);
			Keyboard::__SetKey(KeyCode::LeftCtrl, m & KMOD_LCTRL);
			Keyboard::__SetKey(KeyCode::LeftAlt, m & KMOD_LALT);
			Keyboard::__SetKey(KeyCode::RightShift, m & KMOD_RSHIFT);
			Keyboard::__SetKey(KeyCode::RightCtrl, m & KMOD_RCTRL);
			Keyboard::__SetKey(KeyCode::RightAlt, m & KMOD_RALT);

		}

		void SystemEvents::__UpdateMouse() {
			static int mouseState = 0;
			static ivec2 mousepos = ivec2(0);
			mouseState = SDL_GetMouseState(&mousepos.x, &mousepos.y);

			Mouse::__SetNewPosition(mousepos);
			Mouse::__SetButtons(
				mouseState & SDL_BUTTON(SDL_BUTTON_LEFT),
				mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT),
				mouseState & SDL_BUTTON(SDL_BUTTON_MIDDLE)
			);
		}

	}
}