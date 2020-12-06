#include "BTBoomerang.hpp"

BTBoomerang::BTBoomerang() {}
BTBoomerang::~BTBoomerang() {}

void BTBoomerang::OnCreate(int _count) {

}

void BTBoomerang::Update(Button _shootButton) {
	if (_shootButton.Pressed()) {
		DEBUG_LOG("Boomerang has been fired!");
	}
}