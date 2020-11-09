#include "HomingLasers.hpp"

void HomingLasers::Update(Button _shootButton) {
	if (_shootButton.Pressed()) {
		DEBUG_LOG("Homing lasers have been fired!");
	}
}