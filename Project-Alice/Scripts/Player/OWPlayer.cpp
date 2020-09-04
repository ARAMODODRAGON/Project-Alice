#include "OWPlayer.hpp"

RTTR_REGISTRATION {
	registration::class_<OWPlayer>("OWPlayer")
		.public_object_constructor
		.property("inventory", &OWPlayer::inventory);
}

OWPlayer::OWPlayer() { }

OWPlayer::~OWPlayer() { }

void OWPlayer::Start() { 
	for (size_t i = 0; i < inventory.size(); ++i) {
		DEBUG_LOG("Item 1: " + VTOS(inventory[i]));
	}
}

void OWPlayer::Update() { }

void OWPlayer::LateUpdate() { }

void OWPlayer::OnDestroy() { }
