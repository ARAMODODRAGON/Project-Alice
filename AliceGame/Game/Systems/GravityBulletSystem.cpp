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
		grav.timeToFall = 0.0f;

		if (bb.velocity.y < -300.0f) {
			bb.velocity.y = -300.0f; }
	}  
	
}
