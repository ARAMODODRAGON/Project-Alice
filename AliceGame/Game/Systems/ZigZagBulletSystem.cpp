#include "ZigZagBulletSystem.hpp"

ZigZagBulletSystem::ZigZagBulletSystem()
{
}

ZigZagBulletSystem::~ZigZagBulletSystem()
{
}

void ZigZagBulletSystem::Step(ALC::Timestep ts, ALC::Entity e, ALC::BulletBody& bb, ZigZagBullets& zzBul)
{
	float shiftDist = 250.0f;
	zzBul.timeToShift += ts.Get();

	if (zzBul.timeToShift >= ts.Get() && zzBul.timeToShift <= ts.Get() * 30.0f ) {
		bb.velocity.x = shiftDist;
	}

	else if(zzBul.timeToShift > ts.Get() * 30.0f && zzBul.timeToShift <= (ts.Get() * 60.0f)){
		bb.velocity.x = -shiftDist;
	}

	else {
		zzBul.timeToShift = 0.0f;
		
	}

}
