#include "BTBoomerang.hpp"

RTTR_REGISTRATION{
	registration::class_<BTBoomerang>(BOOMERANG)
		.public_object_constructor;
}

BTBoomerang::BTBoomerang() {}
BTBoomerang::~BTBoomerang() {}

void BTBoomerang::OnCreate(type _type, int _count) {

}

void BTBoomerang::Update(Button _shootButton) {
	if (_shootButton.Pressed()) {
		DEBUG_LOG("Boomerang has been fired!");
	}
}