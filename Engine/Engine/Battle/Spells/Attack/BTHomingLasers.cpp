#include "BTHomingLasers.hpp"

RTTR_REGISTRATION{
	registration::class_<BTHomingLasers>("BTHomingLasers");
}

void BTHomingLasers::Update(Button _shootButton) {
	if (_shootButton.Pressed()) {
		DEBUG_LOG("Homing lasers have been fired!");
	}
}