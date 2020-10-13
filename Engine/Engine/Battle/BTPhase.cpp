#include "BTPhase.hpp"
#include "BTEnemy.hpp"

RTTR_REGISTRATION{
	registration::class_<BTPhase>("BTPhase")
		.property("nextPahse",&BTPhase::nextPhase)
		.property("phaseName",&BTPhase::phaseName)
		.property("currentHealth",&BTPhase::currentHealth)
		.property("startingTime",&BTPhase::startingTime)
		.property("enemy",&BTPhase::enemy)
		.property("enemyAcceleration",&BTPhase::enemyAcceleration)
		.property("enemyMaxSpeed",&BTPhase::enemyMaxSpeed);
}

BTPhase::BTPhase() :nextPhase(""), phaseName(""), currentHealth(0.0f), startingTime(0.0f), enemy(nullptr),enemyAcceleration(0.0f),enemyMaxSpeed(0.0f) {}


