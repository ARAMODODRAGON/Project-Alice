#include "BulletPhysicsHandler.hpp"
#include <glm\gtx\norm.hpp>

BulletPhysicsHandler::BulletPhysicsHandler() { }
BulletPhysicsHandler::~BulletPhysicsHandler() { }

void BulletPhysicsHandler::Step(ALC::Registry& registry, ALC::Timestep t) {

	auto& reg = registry.__GetReg();
	auto charas = reg.view<CharacterBody, ALC::Transform2D, ALC::EntityInfo>(entt::exclude_t<BulletBody>());
	auto bullets = reg.view<BulletBody, ALC::Transform2D, ALC::EntityInfo>(entt::exclude_t<CharacterBody>());

	// update motion

	// characters
	for (const auto e : charas) {
		auto [cb, tr] = charas.get<CharacterBody, ALC::Transform2D>(e);

		// motion
		tr.position += cb.velocity * t.Get();

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
	}

	// bullets
	bullets.each([&](BulletBody& bb, ALC::Transform2D& tr, ALC::EntityInfo& ei) {
		tr.position += bb.velocity * t.Get();
		bb.__SetCol(nullptr);
	});

	// update collisions
	for (const auto c : charas) {
		// characters
		auto [b0, t0, ci] = charas.get<CharacterBody, ALC::Transform2D, ALC::EntityInfo>(c);
		if (!b0.isSimulated || b0.radius < 0.0001f) continue;

		for (const auto b : bullets) {
			// bullets
			auto [b1, t1, bi] = bullets.get<BulletBody, ALC::Transform2D, ALC::EntityInfo>(b);
			if (!b1.isSimulated || b1.radius < 0.0001f) continue;

			// check layermask
			if (!(b0.mask && b1.mask)) continue;

			// determine overlap
			ALC::vec2 dist = t0.position - t1.position;
			float mindist = b0.radius + b1.radius;
			if (glm::length2(dist) < (mindist * mindist)) {
				// create / update collision entry
				auto& collinfo = b0.__GetColl();

				// update entry
				for (size_t i = 0; i < collinfo.size(); i++) {
					if (collinfo[i].GetOther() == bi.GetID()) {
						collinfo[i].__SetState(ALC::CollisionState::Stay);
						continue;
					}
				}

				// new entry
				BCollisionInfo cinfo(0, b0.mask & b1.mask, bi.GetID(), ALC::CollisionState::Begin, b1.damage);
				collinfo.push_back(cinfo);

				// mark the bullet so it knows it collided with this character
				b1.__SetCol(ci.GetID());
				continue;
			}
		} // inner loop
	} // outter loop

}
