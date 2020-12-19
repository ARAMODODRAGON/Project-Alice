#include "Physics2DHandler.hpp"
#include "Collision2D.hpp"

namespace ALC {

	Physics2DHandler::Physics2DHandler() { }

	Physics2DHandler::~Physics2DHandler() { }

	void Physics2DHandler::Step(Registry& registry, Timestep t) {
		// no physics this frame because there is no timestep
		if (NearlyZero(t, 0.001f)) return;

		auto& reg = registry.__GetReg();
		auto group = reg.group<Rigidbody2D, EntityInfo, Transform2D>();
		auto* entities = group.data();

		for (size_t i = 0; i < group.size(); i++) {
			auto [body, info, transform] = group.get<Rigidbody2D, EntityInfo, Transform2D>(entities[i]);

			// do physics
			body.velocity += body.acceleration * t.Get();
			transform.position += body.velocity * t.Get();

			// update collision info
			auto& collinfo = body.__GetCollInfo();
			for (auto it = collinfo.begin(); it != collinfo.end();) {
				if (it->GetState() == CollisionState::End) {
					it = collinfo.erase(it);
					continue;
				}
				it->__SetState(CollisionState::End);
				++it;
			}
		}

		for (size_t i = 0; i < (group.size() - 1); i++) {
			auto [b0, i0, t0] = group.get<Rigidbody2D, EntityInfo, Transform2D>(entities[i]);
			if (b0.GetShapeType() == ShapeType::None) continue;

			for (size_t j = i + 1; j < group.size(); j++) {
				auto [b1, i1, t1] = group.get<Rigidbody2D, EntityInfo, Transform2D>(entities[i + 1]);
				if (b1.GetShapeType() == ShapeType::None) continue;
			
			}
		}

		//// check collisions first
		//if (b1.preferCollisions || b0.preferCollisions) {
		//
		//	// collision
		//	if (b0.collisionMask && b1.collisionMask) {
		//		//ALC_DEBUG_WARNING("Collisions not currently supported");
		//		continue;
		//	}
		//	// trigger
		//	else if (b0.triggerMask && b1.triggerMask) {
		//		if (DoTrigger(t0, b0, t1, b1)) {
		//			UpdateCollisionInfo(i0, b0, i1, b1);
		//		}
		//	}
		//
		//}
		//// check triggers first
		//else {
		//
		//	// trigger
		//	if (b0.triggerMask && b1.triggerMask) {
		//		if (DoTrigger(t0, b0, t1, b1)) {
		//			UpdateCollisionInfo(i0, b0, i1, b1);
		//		}
		//	}
		//	// collision
		//	else if (b0.collisionMask && b1.collisionMask) {
		//		//ALC_DEBUG_WARNING("Collisions not currently supported");
		//		continue;
		//	}
		//
		//}
	}

	bool Physics2DHandler::DoTrigger(
		Transform2D& t0, Rigidbody2D& r0, Transform2D& t1, Rigidbody2D& r1
	) {
		switch (r0.GetShapeType()) {
			case ShapeType::Circle:
				switch (r1.GetShapeType()) {
					case ShapeType::Circle:
						return Collision2D::CircleCircleTrig(
							t0, r0, r0.GetShape<CircleShape>(),
							t1, r1, r1.GetShape<CircleShape>()
						);
					default: break;
				}
				break;
			default: break;
		}
		return false;
	}

	void Physics2DHandler::UpdateCollisionInfo(
		EntityInfo& i0, Rigidbody2D& r0, EntityInfo& i1, Rigidbody2D& r1
	) {
		bool contains;
		Layermask32 colmask = r0.collisionMask & r1.collisionMask;
		Layermask32 trigmask = r0.triggerMask & r1.triggerMask;
		CollisionInfo info0(colmask, trigmask, i1.GetID(), CollisionState::None);

		// check if they already contain a collision
		contains = false;
		auto& colinfo0 = r0.__GetCollInfo();
		for (auto& collision : colinfo0) {
			if (collision.GetOther() == i1.GetID()) {
				collision = info0;
				collision.__SetState(CollisionState::Stay);
				contains = true;
			}
		}
		if (!contains) {
			info0.__SetState(CollisionState::Begin);
			colinfo0.push_back(info0);
		}

		CollisionInfo info1(colmask, trigmask, i0.GetID(), CollisionState::None);

		contains = false;
		auto& colinfo1 = r1.__GetCollInfo();
		for (auto& collision : colinfo1) {
			if (collision.GetOther() == i0.GetID()) {
				collision = info1;
				collision.__SetState(CollisionState::Stay);
				contains = true;
			}
		}
		if (!contains) {
			info1.__SetState(CollisionState::Begin);
			colinfo1.push_back(info1);
		}
	}

}

// do basic physics and update the collision info
//for (auto it = bodies.begin(); it != bodies.end(); ++it) {
//	auto [transform, body] = bodies.get<Transform2D, Rigidbody2D>(*it);
//
//	// do physics
//	body.velocity += body.acceleration * t.Get();
//	transform.position += body.velocity * t.Get();
//
//	// update collision info
//	auto& collinfo = body.__GetCollInfo();
//	for (auto it = collinfo.begin(); it != collinfo.end();) {
//		if (it->GetState() == CollisionState::End) {
//			it = collinfo.erase(it);
//			continue;
//		}
//		it->__SetState(CollisionState::End);
//		++it;
//	}
//}

//// do collisions
//for (auto it0 = bodies.begin(); it0 != bodies.end() && bodies.size() > 1; ++it0) {
//	entt::entity e0 = *it0;
//	//auto& i0 = bodies.get<EntityInfo>(e0);
//	//auto& t0 = bodies.get<Transform2D>(e0);
//	auto& b0 = bodies.get<Rigidbody2D>(e0);
//	//if (b0.GetShapeType() == ShapeType::None) continue;
//
//	auto it1 = it0; ++it1;
//	for (; it1 != bodies.end(); ++it1) {
//		entt::entity e1 = *it1;
//		//auto& i1 = bodies.get<EntityInfo>(e1);
//		//auto& t1 = bodies.get<Transform2D>(e1);
//		auto& b1 = bodies.get<Rigidbody2D>(e1);
//		//if (b1.GetShapeType() == ShapeType::None) continue;
//
//		//// check collisions first
//		//if (b1.preferCollisions || b0.preferCollisions) {
//		//
//		//	// collision
//		//	if (b0.collisionMask && b1.collisionMask) {
//		//		//ALC_DEBUG_WARNING("Collisions not currently supported");
//		//		continue;
//		//	}
//		//	// trigger
//		//	else if (b0.triggerMask && b1.triggerMask) {
//		//		if (DoTrigger(t0, b0, t1, b1)) {
//		//			UpdateCollisionInfo(i0, b0, i1, b1);
//		//		}
//		//	}
//		//
//		//}
//		//// check triggers first
//		//else {
//		//
//		//	// trigger
//		//	if (b0.triggerMask && b1.triggerMask) {
//		//		if (DoTrigger(t0, b0, t1, b1)) {
//		//			UpdateCollisionInfo(i0, b0, i1, b1);
//		//		}
//		//	}
//		//	// collision
//		//	else if (b0.collisionMask && b1.collisionMask) {
//		//		//ALC_DEBUG_WARNING("Collisions not currently supported");
//		//		continue;
//		//	}
//		//
//		//}
//	}
//}

//bodies.each([this, &bodies](entt::entity e0, EntityInfo& i0, Transform2D& t0, Rigidbody2D& b0) {
//	if (b0.GetShapeType() == ShapeType::None) return;
//
//	bodies.each([this, &bodies, e0, &i0, &t0, &b0](entt::entity e1, EntityInfo& i1, Transform2D& t1, Rigidbody2D& b1) {
//		if (static_cast<uint32>(e0) >= static_cast<uint32>(e1)) return;
//		if (b1.GetShapeType() == ShapeType::None) return;
//		
//		// check collisions first
//		if (b1.preferCollisions || b0.preferCollisions) {
//
//			// collision
//			if (b0.collisionMask && b1.collisionMask) {
//				//ALC_DEBUG_WARNING("Collisions not currently supported");
//				//return;
//			}
//			// trigger
//			else if (b0.triggerMask && b1.triggerMask) {
//				if (DoTrigger(t0, b0, t1, b1)) {
//					UpdateCollisionInfo(i0, b0, i1, b1);
//				}
//			}
//
//		}
//		// check triggers first
//		else {
//
//			// trigger
//			if (b0.triggerMask && b1.triggerMask) {
//				if (DoTrigger(t0, b0, t1, b1)) {
//					UpdateCollisionInfo(i0, b0, i1, b1);
//				}
//			}
//			// collision
//			else if (b0.collisionMask && b1.collisionMask) {
//				//ALC_DEBUG_WARNING("Collisions not currently supported");
//				//return;
//			}
//
//		}
//	});
//});
