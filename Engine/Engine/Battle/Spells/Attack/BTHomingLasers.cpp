#include "BTHomingLasers.hpp"

RTTR_REGISTRATION{
	registration::class_<BTHomingLasers>(HOMING_LASERS)
		.public_object_constructor;
}

BTHomingLasers::BTHomingLasers() {}
BTHomingLasers::~BTHomingLasers() {}

void BTHomingLasers::OnCreate(type _type, int _count) {

}

void BTHomingLasers::Update(Button _shootButton) {
	if (_shootButton.Pressed()) {
		DEBUG_LOG("Homing lasers have been fired!");
	}
}