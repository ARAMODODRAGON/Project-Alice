#include "BTHealing.hpp"

RTTR_REGISTRATION{
	registration::class_<BTHealing>("BTHealing");
}

void BTHealing::Use() {
	DEBUG_LOG("Healing has been used!");
}