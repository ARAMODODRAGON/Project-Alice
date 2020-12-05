#ifndef ALC_INPUT_MOUSE_HPP
#define ALC_INPUT_MOUSE_HPP
#include "../General.hpp"
#include "Button.hpp"

namespace ALC {

	class Mouse {
		ALC_NON_CONSTRUCTABLE(Mouse);
	public:

		static Button GetLeft() { return Button(mouseState.leftState, mouseState.leftChanged); }
		static Button GetRight() { return Button(mouseState.rightState, mouseState.rightChanged); }
		static Button GetMiddle() { return Button(mouseState.middleState, mouseState.middleChanged); }
		static vec2 GetPosition() { return mouseState.newPos; }
		static vec2 GetAxis() { return mouseState.posDiff; }

	public:
		static void __SetNewPosition(const ivec2& pos);
		static void __SetButtons(bool left, bool right, bool middle);
		struct MouseState {
			union {
				struct {
					uint8 leftState : 1;
					uint8 leftChanged : 1;
					uint8 rightState : 1;
					uint8 rightChanged : 1;
					uint8 middleState : 1;
					uint8 middleChanged : 1;
				};
				uint8 allStatesChanges;
			};
			vec2 oldPos;
			vec2 newPos;
			vec2 posDiff;
			MouseState();
		};
		static MouseState mouseState;
	};

}

#endif // !ALC_INPUT_MOUSE_HPP