#include "BTHomingLasers.hpp"

BTHomingLasers::BTHomingLasers() {}
BTHomingLasers::~BTHomingLasers() {}

void BTHomingLasers::OnCreate(int _count) {

}

void BTHomingLasers::Update(Button _shootButton) {
	if (_shootButton.Pressed()) {
		DEBUG_LOG("Homing lasers have been fired!");
	}
}