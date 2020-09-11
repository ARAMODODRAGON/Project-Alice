#include "Component.hpp"
#include "Object.hpp"

RTTR_REGISTRATION {
	registration::class_<Component>("Component")
		.property("object", &Component::object)
		.property("isActive", &Component::isActive);
}

Component::Component()
	: object(nullptr)
	, isActive(true) { }
