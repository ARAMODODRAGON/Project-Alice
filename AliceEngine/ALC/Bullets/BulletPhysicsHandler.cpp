#include "BulletPhysicsHandler.hpp"
#include <glm\gtx\norm.hpp>

namespace ALC {

	BulletPhysicsHandler::BulletPhysicsHandler() { }
	BulletPhysicsHandler::~BulletPhysicsHandler() { }

	void BulletPhysicsHandler::Step(Registry& registry, Timestep t) {

		auto& reg = registry.__GetReg();
		auto charas = reg.view<CharacterBody, Transform2D, EntityInfo>(entt::exclude_t<BulletBody>());
		auto bullets = reg.view<BulletBody, Transform2D, EntityInfo>(entt::exclude_t<CharacterBody>());

		// update motion

		// characters
		for (const auto e : charas) {
			auto [cb, tr] = charas.get<CharacterBody, Transform2D>(e);

			// motion
			tr.position += cb.velocity * t.Get();

			// set the list of collisions 
			auto& collisioninfo = cb.__GetColl();
			for (auto it = collisioninfo.begin(); it != collisioninfo.end();) {
				if (it->GetState() == CollisionState::End) {
					it = collisioninfo.erase(it);
					continue;
				}
				it->__SetState(CollisionState::End);
				++it;
			}
		}

		// bullets
		bullets.each([&](BulletBody& bb, Transform2D& tr, EntityInfo& ei) {
			tr.position += bb.velocity * t.Get();
			bb.__SetCol(nullptr);
		});

		// update collisions
		for (const auto c : charas) {
			// characters
			auto [b0, t0, ci] = charas.get<CharacterBody, Transform2D, EntityInfo>(c);
			if (!b0.isSimulated || b0.radius < 0.0001f) continue;

			for (const auto b : bullets) {
				// bullets
				auto [b1, t1, bi] = bullets.get<BulletBody, Transform2D, EntityInfo>(b);
				if (!b1.isSimulated || b1.radius < 0.0001f) continue;

				// check layermask
				if (!(b0.mask && b1.mask)) continue;

				// determine overlap
				vec2 dist = t0.position - t1.position;
				float mindist = b0.radius + b1.radius;
				if (glm::length2(dist) < (mindist * mindist)) {
					// create / update collision entry
					auto& collinfo = b0.__GetColl();

					// update entry
					for (size_t i = 0; i < collinfo.size(); i++) {
						if (collinfo[i].GetOther() == bi.GetID()) {
							collinfo[i].__SetState(CollisionState::Stay);
							continue;
						}
					}

					// new entry
					BCollisionInfo cinfo(0, b0.mask & b1.mask, bi.GetID(), CollisionState::Begin, b1.damage);
					collinfo.push_back(cinfo);

					// mark the bullet so it knows it collided with this character
					b1.__SetCol(ci.GetID());
					continue;
				}
			} // inner loop
		} // outter loop

	}

}