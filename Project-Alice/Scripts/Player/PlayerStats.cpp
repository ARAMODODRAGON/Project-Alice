#include "PlayerStats.hpp"

RTTR_REGISTRATION {
	registration::class_<PlayerStats>("PlayerStats")
		.public_object_constructor;
}

PlayerStats::PlayerStats() { }

PlayerStats::~PlayerStats() { }

void PlayerStats::Start() {
	DEBUG_LOG("My name is " + GetName());
}
