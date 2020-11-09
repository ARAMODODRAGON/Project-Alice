#include "BTHealing.hpp"

RTTR_REGISTRATION{
	registration::class_<BTHealing>(HEALING)
		.public_object_constructor;
}

BTHealing::BTHealing() {}
BTHealing::~BTHealing() {}

void BTHealing::Use() {
	DEBUG_LOG("Healing has been used!");
}