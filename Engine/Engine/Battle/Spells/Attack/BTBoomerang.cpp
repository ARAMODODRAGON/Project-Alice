#include "BTBoomerang.hpp"

RTTR_REGISTRATION{
	registration::class_<BTBoomerang>("BTBoomerang");
}

void BTBoomerang::Update(Button _shootButton) {
	if (_shootButton.Pressed()) {
		DEBUG_LOG("Boomerang has been fired!");
	}
}