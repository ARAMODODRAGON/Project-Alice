#include "BaseBulletSystem.hpp"

namespace ALC {

	BaseBulletSystem::BaseBulletSystem(EntityCreationHandler& ech_)
		: ech(ech_), bounds(std::numeric_limits<float>::infinity()) { }

	void BaseBulletSystem::Step(Timestep ts, const Entity e, BulletComponent& bc, BulletBody& bb, Transform2D& tr) {
		if(!bc.isAlive) return;
		
		if (bc.lifetime > 0.0f) {
			bc.lifetime -= ts;
			if (bc.lifetime <= 0.0f) {
				bc.isAlive = false;
				ech.Destroy(e);
				return;
			}
		}
		else if (tr.position.x < -bounds.x || tr.position.x > bounds.x ||
				 tr.position.y < -bounds.y || tr.position.y > bounds.y) {
			bb.isSimulated = false;
			ech.Destroy(e);
		}

		if (bc.basebulletState == 0) {
			bc.basebulletState = 1;
			bc.realVelocity = bb.velocity;
		}

		if (bc.basebulletState == 1) {
			bb.velocity = bc.realVelocity * bc.speedMult;
			bc.speedMult -= bc.speedReduction * ts;
			if (bc.speedMult < 1.0f) {
				bc.speedMult = 1.0f;
				bc.basebulletState = 2;
				bb.velocity = bc.realVelocity;
			}
		}

	}

}