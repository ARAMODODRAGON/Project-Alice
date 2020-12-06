#include "CircleCollider.hpp"

CircleCollider::CircleCollider() : radius(0.5f) { }

CircleCollider::~CircleCollider() { }

BoundingCircle CircleCollider::GetBounds() const {
	return BoundingCircle(GetObject()->GetPosition() + this->GetPosition(), radius);
}
