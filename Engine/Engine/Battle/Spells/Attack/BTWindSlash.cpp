#include "BTWindSlash.hpp"

BTWindSlash::BTWindSlash() {}
BTWindSlash::~BTWindSlash() {}

void BTWindSlash::OnCreate(int _count) {

}

void BTWindSlash::Update(Button _shootButton) {
	if (_shootButton.Pressed()) {
		DEBUG_LOG("Wind slash has been fired!");
	}
}