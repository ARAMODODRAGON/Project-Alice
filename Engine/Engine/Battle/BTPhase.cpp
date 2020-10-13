#include "BTPhase.hpp"


RTTR_REGISTRATION{
	registration::class_<BTPhase>("BTPhase")
		.property("nextPahse",&BTPhase::nextPhase)
		.property("phaseName",&BTPhase::phaseName)
		.property("currentHealth",&BTPhase::currentHealth)
		.property("maxHealth",&BTPhase::maxHealth)
		.property("startingTime",&BTPhase::startingTime)
		.property("enemy",&BTPhase::enemy)
		.property("acceleration",&BTPhase::acceleration)
		.property("maxSpeed",&BTPhase::maxSpeed)
		.property("destination",&BTPhase::destination)
		.property("position",&BTPhase::position);
}

BTPhase::BTPhase()
	: nextPhase("")
	, phaseName("")
	, currentHealth(0.0f)
	, maxHealth(0.0f)
	, startingTime(0.0f)
	, enemy(nullptr)
	, acceleration(0.0f)
	, maxSpeed(0.0f)
	, destination(vec2())
	, position(vec2())
{}

BTPhase::~BTPhase()
{
	delete enemy;
	enemy = nullptr;
}



