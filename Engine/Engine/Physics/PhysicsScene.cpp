#include "PhysicsScene.hpp"
#include "../Game.hpp"

PhysicsScene::PhysicsScene() { }

PhysicsScene::~PhysicsScene() { }

void PhysicsScene::AddObject(Object* obj) { 
	Get()->objects.push_back(ObjectPairType(obj, 1u));
}

void PhysicsScene::RemoveObject(Object* obj) { 
	Get()->objects.remove_if([obj] (ObjectPairType& p) {
		return obj == p.first;
	});
}

void PhysicsScene::AddComponent(ColliderComponent* colliderComponent) {
	Get()->colliderComponents.push_front(colliderComponent);
}

void PhysicsScene::RemoveComponent(ColliderComponent* colliderComponent) {
	Get()->colliderComponents.remove(colliderComponent);
}

void PhysicsScene::Init() { }

void PhysicsScene::Exit() {
	Get()->objects.clear();
	Get()->colliderComponents.clear();
}

void PhysicsScene::Step() {
	const uint32 fps = Game::Get()->GetTimer()->GetFPS();
	const float delta = float(1.0 / double(fps));
	// get pointer
	auto* inst = Get();

	// do physics
	for (ObjectPairType& obj : inst->objects) {
		obj.first->SetPosition(
			obj.first->GetPosition() + (obj.first->GetVelocity() * delta)
		);
	}

	// set false to track what collisions happened this frame
	for (Collider* coll : inst->colliderComponents) {
		for (pair<CollisionData, unsigned int>& p : coll->collisions) {
			p.second = 0;
		}
	}

	// do collisions
	for (auto it0 = inst->colliderComponents.begin(); it0 != inst->colliderComponents.end(); ++it0) {
		// grab collider and list of collisions
		ColliderComponent* c0 = *it0;

		// get next
		auto it1 = it0; ++it1;
		for (; it1 != inst->colliderComponents.end(); ++it1) {
			// grab collider and list of collisions
			ColliderComponent* c1 = *it1;

			// check if their layermasks match bounding volumes are overlapping
			if (BoundingCircle::Intersects(c0->GetBounds(), c1->GetBounds())) {
				bool result = false;
				bool wasOverlap = false;

				if (c0->triggerMask || c1->triggerMask) {
					wasOverlap = true;
					result = DoOverlap(c0, c1);
				} else if (c0->collisionMask || c1->collisionMask) {
					wasOverlap = false;
					result = DoCollision(c0, c1);
				}

				// if true then there was a collision
				if (result) {

					// first get the collision data from the list and update it (TODO)
					auto it0 = FindCollision(c0->collisions, c0, c1);
					auto it1 = FindCollision(c1->collisions, c0, c1);

					LayerMask collisionMask = 0;
					LayerMask triggerMask = 0;
					if (wasOverlap) triggerMask = c0->triggerMask & c1->triggerMask;
					else			collisionMask = c0->collisionMask & c1->collisionMask;

					// new collision
					if (it0 == c0->collisions.end() && it1 == c1->collisions.end()) {
						// create collision data and add to list
						CollisionData d;
						d.thisCollider = c0;
						d.otherCollider = c1;
						d.collisionMask = collisionMask;
						d.triggerMask = triggerMask;
						c0->collisions.push_back(pair<CollisionData, unsigned int>(d, 2));
						d.thisCollider = c1;
						d.otherCollider = c0;
						c1->collisions.push_back(pair<CollisionData, unsigned int>(d, 2));
					}
					// special case where the collision data doesnt match
					else if (it0 == c0->collisions.end() || it1 == c1->collisions.end()) {
						if (it0 == c0->collisions.end()) {
							c0->collisions.erase(it0);
						}
						if (it1 == c1->collisions.end()) {
							c1->collisions.erase(it1);
						}

						DEBUG_ERROR("Collision error. Mismatch collision data.");
					}
					// old collision
					else {
						// update masks and set to "stay"
						it0->first.collisionMask = collisionMask;
						it0->first.triggerMask = triggerMask;
						it0->second = 1;
						it1->first.collisionMask = collisionMask;
						it1->first.triggerMask = triggerMask;
						it1->second = 1;
					}

				}
			} // intersection
		} // inner for loop
	} // outter for loop

	// collision callbacks
	for (ColliderComponent* c : inst->colliderComponents) {
		Object* o = c->GetObject();

		// enter callback
		for (auto& p : c->collisions) {
			if (p.second == 2) {
				o->OnCollisionEnter(p.first);
			}
		}
		// stay callback
		for (auto& p : c->collisions) {
			if (p.second == 1) {
				o->OnCollisionStay(p.first);
			}
		}
		// exit callback
		for (auto it = c->collisions.begin(); it != c->collisions.end(); ++it) {
			if (it->second == 0) {
				o->OnCollisionExit(it->first);
				it = c->collisions.erase(it);
				// double check
				if (it == c->collisions.end()) break;
			}
		}
	}
}

PhysicsScene::CollDataPairIter PhysicsScene::FindCollision(
	list<pair<CollisionData, unsigned int>>& collisions,
	Collider* c0,
	Collider* c1
) {
	for (auto it = collisions.begin(); it != collisions.end(); ++it) {
		if (it->first.thisCollider == c0) {
			if (it->first.otherCollider == c1) {
				return it;
			}
		} else if (it->first.thisCollider == c1) {
			if (it->first.otherCollider == c0) {
				return it;
			}
		}
	}

	return collisions.end();
}

bool PhysicsScene::DoCollision(ColliderComponent* c0, Collider* c1) {
	return false; // everything else false
}

bool PhysicsScene::DoOverlap(ColliderComponent* c0, Collider* c1) {
	switch (c0->GetColType()) {
		case ColType::Circle:
			switch (c1->GetColType()) {
				case ColType::Circle:
					// circles only do overlaps
					return true;
				default: return false; // everything else false
			}
		default: return false; // everything else false
	}
}
