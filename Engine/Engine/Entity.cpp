#include "Entity.hpp"

RTTR_REGISTRATION {
	registration::class_<Entity>("Entity")
		.property("isActive", &Entity::isActive)
		.property("name", &Entity::name)
		.property("position", &Entity::position)
		.property("velocity", &Entity::velocity)
		.property("isVisible", &Entity::isVisible)
		.property("spriteIndex", &Entity::spriteIndex)
		.property("pivot", &Entity::pivot)
		.property("scale", &Entity::scale)
		.property("rotation", &Entity::rotation)
		.property("layer", &Entity::layer)
		.property("isSimulated", &Entity::isSimulated)
		.property("isAnchored", &Entity::isAnchored)
		.property("colliderRadius", &Entity::colliderRadius);
}

// TODO: add VAO code
Entity::Entity()
	: level(nullptr)
	, isActive(true)
	, name("Entity")
	, isVisible(true)
	, spriteIndex(0)
	, rotation(0.0f)
	, layer(0)
	, VAO(-1), VBO(-1), EBO(-1)
	, isSimulated(true)
	, isAnchored(false)
	, colliderRadius(0.5f) { }

Entity::~Entity() { }

