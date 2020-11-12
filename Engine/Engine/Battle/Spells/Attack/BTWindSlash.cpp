#include "BTWindSlash.hpp"

RTTR_REGISTRATION{
	registration::class_<BTWindSlash>(WIND_SLASH)
		.public_object_constructor;
}

BTWindSlash::BTWindSlash() {}
BTWindSlash::~BTWindSlash() {}

void BTWindSlash::OnCreate(type _type, int _count) {

}

void BTWindSlash::Update(Button _shootButton) {
	if (_shootButton.Pressed()) {
		DEBUG_LOG("Wind slash has been fired!");
	}
}