#ifndef _PHYSICS_COLLIDER_COMPONENT_HPP
#define _PHYSICS_COLLIDER_COMPONENT_HPP
#include "Colliders.hpp"
#include "../Objects/Object.hpp"

class ColliderComponent : public Component, public Collider {

public:

	ColliderComponent();
	virtual ~ColliderComponent() = 0;

	// events
	void Start() override;
	void OnDestroy() override;


private:

	friend class PhysicsScene;


};

#endif // !_PHYSICS_COLLIDER_COMPONENT_HPP