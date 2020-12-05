#include "Mouse.hpp"

namespace ALC {

	Mouse::MouseState Mouse::mouseState;

	Mouse::MouseState::MouseState()
		: allStatesChanges(0) { }

	void Mouse::__SetNewPosition(const ivec2& pos) {
		mouseState.oldPos = mouseState.newPos;
		mouseState.newPos = pos;
		mouseState.posDiff = mouseState.newPos - mouseState.oldPos;
	}

	void Mouse::__SetButtons(bool left, bool right, bool middle) {
		if (mouseState.leftState != left) mouseState.leftChanged = true;
		else mouseState.leftChanged = false;
		mouseState.leftState = left;

		if (mouseState.rightState != right) mouseState.rightChanged = true;
		else mouseState.rightChanged = false;
		mouseState.rightState = right;

		if (mouseState.middleState != middle) mouseState.middleChanged = true;
		else mouseState.middleChanged = false;
		mouseState.middleState = middle;
	}

}