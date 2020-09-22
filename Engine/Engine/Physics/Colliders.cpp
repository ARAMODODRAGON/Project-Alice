#include "Colliders.hpp"

RTTR_REGISTRATION {
	registration::class_<Collider>("Collider")
		.property("position", &Collider::position)
		.property_readonly("coltype", &Collider::coltype);
}
