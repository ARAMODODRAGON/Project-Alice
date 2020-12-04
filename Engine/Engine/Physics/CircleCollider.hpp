#ifndef _PHYSICS_CIRCLE_COLLIDER_HPP
#define _PHYSICS_CIRCLE_COLLIDER_HPP
#include "ColliderComponent.hpp"
// inherits both component and collider

class CircleCollider : public ColliderComponent {

	float radius;

public:

	CircleCollider();
	~CircleCollider();

	// getters & setters
	float GetRadius() const { return radius; }
	void SetRadius(float radius_) { if (radius_ < 0.0f) radius = 0.0f; else radius = radius_; } // makes sure its not set to < 0
	virtual ColType GetColType() const override { return ColType::Circle; }
	virtual BoundingCircle GetBounds() const override;

};

#endif // !_PHYSICS_CIRCLE_COLLIDER_HPP