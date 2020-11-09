#include "BTAbsoluteBarrier.hpp"

RTTR_REGISTRATION{
	registration::class_<BTAbsoluteBarrier>("BTAbsoluteBarrier");
}

void BTAbsoluteBarrier::Use() {
	DEBUG_LOG("Absolute Barrier has been used!");
}