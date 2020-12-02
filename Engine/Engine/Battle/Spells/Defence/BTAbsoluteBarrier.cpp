#include "BTAbsoluteBarrier.hpp"

RTTR_REGISTRATION{
	registration::class_<BTAbsoluteBarrier>(ABSOLUTE_BARRIER)
		.public_object_constructor;
}

BTAbsoluteBarrier::BTAbsoluteBarrier() {}
BTAbsoluteBarrier::~BTAbsoluteBarrier() {}

void BTAbsoluteBarrier::Use() {
	DEBUG_LOG("Absolute Barrier has been used!");
}