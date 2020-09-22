#ifndef _PHYSICS_COLLIDER_COMPONENT_HPP
#define _PHYSICS_COLLIDER_COMPONENT_HPP
#include "Colliders.hpp"
#include "../Objects/Object.hpp"

class ColliderComponent : public Component, public Collider {

public:

	ColliderComponent(const ColType coltype);
	~ColliderComponent();

	// events
	void Start() override;
	void OnDestroy() override;

	RTTR_ENABLE(Component, Collider) RTTR_REGISTRATION_FRIEND

private:

	friend class PhysicsScene;

	void InvokeCollisionCallbacks();

};

#endif // !_PHYSICS_COLLIDER_COMPONENT_HPP