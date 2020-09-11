#include "OWPlayer.hpp"

RTTR_REGISTRATION {
	registration::class_<OWPlayer>("OWPlayer")
		.public_object_constructor;
}

OWPlayer::OWPlayer() { }

OWPlayer::~OWPlayer() { }

void OWPlayer::Start() { 
	DEBUG_LOG("My name is " + GetName() + " and I am at: " + VTOS(GetPosition()));
}

