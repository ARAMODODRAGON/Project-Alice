#include "BTWindSlash.hpp"

RTTR_REGISTRATION{
	registration::class_<BTWindSlash>("BTWindSlash");
}

void BTWindSlash::Update(Button _shootButton) {
	if (_shootButton.Pressed()) {
		DEBUG_LOG("Wind slash has been fired!");
	}
}