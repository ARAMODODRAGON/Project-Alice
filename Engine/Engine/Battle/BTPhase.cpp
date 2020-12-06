#include "BTPhase.hpp"

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



