#include "BulletPhysicsSystem.hpp"

BulletPhysicsSystem::BulletPhysicsSystem(ALC::Registry& reg)
	: m_reg(reg) { }

BulletPhysicsSystem::~BulletPhysicsSystem() { }

void BulletPhysicsSystem::UpdateCharacters(ALC::Timestep ts) {
	size_t i = 0;
	m_reg.ForeachComponent<CharacterBody, ALC::Transform2D>(
		[this, ts, &i](ALC::Entity e, CharacterBody& cb, ALC::Transform2D& tr) {
		// set index in charpairs
		if (m_charpairs.size() <= i) m_charpairs.emplace_back();
		m_charpairs[i++].entity = e;

		// motion
		tr.position += cb.velocity * ts;

		// set the list of collisions 
		auto& collisioninfo = cb.__GetColl();
		for (auto it = collisioninfo.begin(); it != collisioninfo.end();) {
			if (it->GetState() == ALC::CollisionState::End) {
				it = collisioninfo.erase(it);
				continue;
			}
			it->__SetState(ALC::CollisionState::End);
			++it;
		}
	});
	if (i < m_charpairs.size()) m_charpairs.resize(i);
}

void BulletPhysicsSystem::Step(ALC::Timestep ts, ALC::Entity e, BulletBody& bb, ALC::Transform2D& tr, ALC::EntityInfo& bi) {
	if (!bb.isSimulated || bb.radius < 0.0001f) return;

	tr.position += bb.velocity * ts;
	bb.__SetCol(nullptr);

	// test for collisions
	for (size_t i = 0; i < m_charpairs.size(); i++) {
		auto& chp = m_charpairs[i];
		if (!chp.entity) return;

		auto [ct, cb] = chp.entity.GetComponent<ALC::Transform2D, CharacterBody>();
		if (!cb.isSimulated || cb.radius < 0.0001f) continue;

		// check layermask
		if (!(cb.mask && bb.mask)) continue;

		// determine overlap
		if (float mindist = bb.radius + cb.radius;
			glm::length2(tr.position - ct.position) >= (mindist * mindist)) continue;

		// lock and write collision info
		cjs::mutex_guard _(chp.lock);

		// get some stuff
		auto& ci = chp.entity.GetComponent<ALC::EntityInfo>();
		auto& collinfo = cb.__GetColl();

		// update entry
		for (size_t i = 0; i < collinfo.size(); i++) {
			if (collinfo[i].GetOther() == bi.GetID()) {
				collinfo[i].__SetState(ALC::CollisionState::Stay);
				break;
			}
		}

		// new entry
		BCollisionInfo cinfo(0, bb.mask & cb.mask, bi.GetID(), ALC::CollisionState::Begin, bb.damage);
		collinfo.push_back(cinfo);

		// mark the bullet so it knows it collided with this character
		bb.__SetCol(ci.GetID());

		return;
	}
}
