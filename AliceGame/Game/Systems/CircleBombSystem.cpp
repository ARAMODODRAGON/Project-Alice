#include "CircleBombSystem.hpp"
#include "../Characters/Character.hpp"
#include <ALC\Bullets.hpp>
#include <ALC/Bullets/ShooterBehavior.hpp>

CircleBombSystem::CircleBombSystem()
{
}

CircleBombSystem::~CircleBombSystem()
{
}

void CircleBombSystem::Step(ALC::Timestep ts, ALC::Entity e, ALC::BulletBody& bb, CircleBomb& cirBmb)
{	
	if (cirBmb.canSetVel) {
		cirBmb.initVel = bb.velocity;
		cirBmb.canSetVel = false;
	}

	cirBmb.timeToExld -= ts.Get();
	float pushTime = .3f; //the time that the bullets should stop moving after being fired 
	cirBmb.timeToStop += ts.Get();

	
	if (cirBmb.timeToStop >= pushTime) {

		bb.velocity = ALC::vec2();

		if (cirBmb.timeToExld <= 0.0f) {
			bb.velocity = cirBmb.initVel;
		}
	}
}
