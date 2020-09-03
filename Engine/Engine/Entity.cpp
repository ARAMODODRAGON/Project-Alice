#include "Entity.hpp"

RTTR_REGISTRATION {
	registration::class_<Entity>("Entity")
		.constructor<>()
		.property("isActive", &Entity::isActive)
		.property("name", &Entity::name)
		.property("position", &Entity::position)
		.property("isVisible", &Entity::isVisible)
		.property("spriteIndex", &Entity::spriteIndex)
		.property("pivot", &Entity::pivot)
		.property("scale", &Entity::scale)
		.property("rotation", &Entity::rotation)
		.property("layer", &Entity::layer)
		.property("isSimulated", &Entity::isSimulated)
		.property("isAnchored", &Entity::isAnchored)
		.property("velocity", &Entity::velocity)
		.property("colliderRadius", &Entity::colliderRadius);
}

Entity::Entity()
	: isActive(true)
	, name("Entity")
	, isVisible(true)
	, spriteIndex(0)
	, rotation(0.0f)
	, layer(0)
	, isSimulated(true)
	, isAnchored(false)
	, colliderRadius(0.5f) { }

Entity::~Entity() { }
