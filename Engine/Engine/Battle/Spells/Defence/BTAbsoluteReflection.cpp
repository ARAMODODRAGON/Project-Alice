#include "BTAbsoluteReflection.hpp"

RTTR_REGISTRATION{
	registration::class_<BTAbsoluteReflection>("BTAbsoluteReflection");
}

void BTAbsoluteReflection::Use() {
	DEBUG_LOG("Absolute Reflection has been used!");
}