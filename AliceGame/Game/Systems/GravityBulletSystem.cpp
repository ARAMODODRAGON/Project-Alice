#include "GravityBulletSystem.hpp"

GravityBulletSystem::GravityBulletSystem()
{
}

GravityBulletSystem::~GravityBulletSystem()
{
}


void GravityBulletSystem::Step(ALC::Timestep ts, ALC::Entity e, BulletBody& bb, GravityBullet& grav)
{
	grav.timeToFall += ts.Get();
	//when the bullet has reached its time to fall add a constant downward velocity to the bullets current velocity 
	if (grav.timeToFall >= .1f) {
		bb.velocity += gravity;
		grav.timeToFall = 0.0f;

		if (bb.velocity.y < -300.0f) {
			bb.velocity.y = -300.0f; }
	}  
	
}
