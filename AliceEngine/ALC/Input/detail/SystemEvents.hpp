#ifndef ALC_EVENTS_SYSTEM_EVENTS_HPP
#define ALC_EVENTS_SYSTEM_EVENTS_HPP
#include "../../General.hpp"

struct SDL_KeyboardEvent;

namespace ALC {
	namespace detail {

		class SystemEvents {
			ALC_NON_CONSTRUCTABLE(SystemEvents)
		public:
			static void PollEvents();
			static void __KeyboardEvent(SDL_KeyboardEvent& ke);
			static void __UpdateKeyMods();
			static void __UpdateMouse();
		};

	}
}

#endif // !ALC_EVENTS_SYSTEM_EVENTS_HPP