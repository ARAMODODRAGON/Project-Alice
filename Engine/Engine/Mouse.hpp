#ifndef __INPUT_MOUSE_HPP
#define __INPUT_MOUSE_HPP
#include "General/Macros.hpp"
#include "General/Math.hpp"
#include "General/Types.hpp"
#include "Button.hpp"

class Mouse {
	NON_CONSTRUCTABLE(Mouse);
	friend class SystemEvents; // SystemEvents modifies this class

	struct MouseState {
		union {
			struct {
				ubyte leftState : 1;
				ubyte leftChanged : 1;
				ubyte rightState : 1;
				ubyte rightChanged : 1;
				ubyte middleState : 1;
				ubyte middleChanged : 1;
			};
			ubyte allStatesChanges;
		};

		vec2 oldPos;
		vec2 newPos;
		vec2 posDiff;

		MouseState();

	};

	static MouseState mouseState;

public:

	/// getters

	static Button GetLeft() { return Button(mouseState.leftState, mouseState.leftChanged); }
	static Button GetRight() { return Button(mouseState.rightState, mouseState.rightChanged); }
	static Button GetMiddle() { return Button(mouseState.middleState, mouseState.middleChanged); }
	static vec2 GetPosition() { return mouseState.newPos; }
	static vec2 GetAxis() { return mouseState.posDiff; }

private:

	/// functions

	static void SetNewPosition(const ivec2& pos);
	static void SetButtons(bool left, bool right, bool middle);

};

#endif // !__INPUT_MOUSE_HPP