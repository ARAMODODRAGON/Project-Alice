#include "WindSlash.hpp"

void WindSlash::Update(Button _shootButton) {
	if (_shootButton.Pressed()) {
		DEBUG_LOG("Wind slash has been fired!");
	}
}