#include "BTPhase.hpp"
#include "BTEnemy.hpp"

RTTR_REGISTRATION{
	registration::class_<BTPhase>("BTPhase")
		.property("nextPahse",&BTPhase::nextPhase)
		.property("phaseName",&BTPhase::phaseName)
		.property("currentHealth",&BTPhase::currentHealth)
		.property("startingTime",&BTPhase::startingTime)
		.property("enemy",&BTPhase::startingTime);
}

BTPhase::BTPhase() :nextPhase(""), phaseName(""), currentHealth(NULL), startingTime(0.0f), enemy(nullptr) {}


