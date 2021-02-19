#include "GravityBulletSystem.hpp"

GravityBulletSystem::GravityBulletSystem()
{
}

GravityBulletSystem::~GravityBulletSystem()
{
}


void GravityBulletSystem::Step(ALC::Timestep ts, ALC::Entity e, ALC::BulletBody& bb, GravityBullet& grav)
{
	grav.timeToFall += ts.Get();

	if (grav.timeToFall >= .1f) {
		bb.velocity += gravity;
		ALC::Debugger::Log("grav bull called", "", __LINE__);
		grav.timeToFall = 0.0f;
	}
	
}
