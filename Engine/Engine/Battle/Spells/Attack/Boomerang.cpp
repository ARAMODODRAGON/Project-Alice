#include "Boomerang.hpp"

void Boomerang::Update(Button _shootButton) {
	if (_shootButton.Pressed()) {
		DEBUG_LOG("Boomerang has been fired!");
	}
}