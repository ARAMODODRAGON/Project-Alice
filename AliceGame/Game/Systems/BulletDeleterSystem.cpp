#include "BulletDeleterSystem.hpp"


BulletDeleterSystem::BulletDeleterSystem(ALC::EntityCreationHandler& ech)
	: m_ech(ech), m_deathBoundry(ALC::vec2(0.0f), ALC::vec2(0.0f)) { }

void BulletDeleterSystem::SetDeathBoundry(const ALC::rect& deathBoundry) {
	m_deathBoundry = deathBoundry;
}

void BulletDeleterSystem::Step(ALC::Timestep ts, ALC::Entity e, BulletDeleterComponent& bdc, BulletBody& bb) {
	if (!bb.isSimulated) return;

	// kill by collision
	auto ci = bb.GetLastCollision();
	if (bdc.deleteOnContact && ci.IsValid()) {
		//m_ech.Destroy(e);
		bb.isSimulated = false;
	}
	// kill by boundry
	else if (bdc.lifetime <= 0.0f) {
		// ignore if the boundry is 0 in size
		if (ALC::NearlyEqual(m_deathBoundry.min, m_deathBoundry.max))
			return;

		// delete if out of bounds
		else {
			auto [ts, spr] = e.GetComponent<ALC::Transform2D, ALC::SpriteComponent>();
			// get the bounds of the bullet
			ALC::rect bounds = spr.bounds;
			bounds.min += ts.position / 2.0f;
			bounds.max += ts.position / 2.0f;

			// delete if its out of bounds
			if (!ALC::rect::Intersects(bounds, m_deathBoundry)) {
				//m_ech.Destroy(e);
				bb.isSimulated = false;
			}
		}
	}
	// kill by time
	else if (bdc.lifetime > 0.0f) {
		bdc.lifetime -= ts;
		if (bdc.lifetime <= 0.0f) {
			//m_ech.Destroy(e);
			bb.isSimulated = false;
		}
	}

}
