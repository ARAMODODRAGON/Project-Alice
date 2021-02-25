#include "ZigZagBulletSystem.hpp"

ZigZagBulletSystem::ZigZagBulletSystem()
{
}

ZigZagBulletSystem::~ZigZagBulletSystem()
{
}

void ZigZagBulletSystem::Step(ALC::Timestep ts, ALC::Entity e, ALC::BulletBody& bb, ZigZagBullets& zzBul)
{
	zzBul.timeToShift += ts.Get();
	
	if (zzBul.timeToShift > 1.1f) {
		e.GetComponent<ALC::Transform2D>().position.x = (e.GetComponent<ALC::Transform2D>().position.x + 5.0f);
		//zzBul.timeToShift = 0.0f;
	}
	else {

		zzBul.timeToShift = 0.0f;						
	}

	/*zzBul.lifeSpan -= ts.Get();

	if (zzBul.lifeSpan <= 0.0f) {
		bb.isSimulated = false;
	} */
}
