#include "BTAbsoluteReflection.hpp"

RTTR_REGISTRATION{
	registration::class_<BTAbsoluteReflection>(ABSOLUTE_REFLECTION)
		.public_object_constructor;
}

BTAbsoluteReflection::BTAbsoluteReflection() {}
BTAbsoluteReflection::~BTAbsoluteReflection() {}

void BTAbsoluteReflection::Use() {
	DEBUG_LOG("Absolute Reflection has been used!");
}