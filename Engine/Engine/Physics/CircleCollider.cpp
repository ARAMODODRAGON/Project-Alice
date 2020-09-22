#include "CircleCollider.hpp"

RTTR_REGISTRATION {
	registration::class_<CircleCollider>("CircleCollider")
		// getter & setter here since we want to keep the value above 0
		.property("radius", &CircleCollider::GetRadius, &CircleCollider::SetRadius);
}

CircleCollider::CircleCollider() : radius(0.5f) { }

CircleCollider::~CircleCollider() { }

BoundingCircle CircleCollider::GetBounds() const {
	return BoundingCircle(GetObject()->GetPosition() + this->GetPosition(), radius);
}
