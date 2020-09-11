#include "Object.hpp"

RTTR_REGISTRATION {
	registration::class_<Object>("Object")
		.public_object_constructor
		.property("factory", &Object::factory)
		.property("isActive", &Object::isActive)
		.property("name", &Object::name)
		.property("position", &Object::position)
		.property("velocity", &Object::velocity)
		.property("scale", &Object::scale)
		.property("rotation", &Object::rotation)
		.property("components", &Object::components);
}

// TODO: add VAO code
Object::Object()
	: factory(nullptr)
	, isActive(true)
	, name("Object")
	, rotation(0.0f) { }

Object::~Object() { }

Object* Object::Make() {
	if (factory)
		return factory->Make<Object>();
	else {
		DEBUG_ERROR("No factory attached to the entity " + name);
		return nullptr;
	}
}

Object* Object::Make(const string& objectName) {
	if (factory)
		return factory->Make<Object>(objectName);
	else {
		DEBUG_ERROR("No factory attached to the entity " + name);
		return nullptr;
	}
}

void Object::Destroy(Object* entity) {
	if (entity->factory) return entity->factory->Destroy(entity);
	else DEBUG_ERROR("No factory attached to the entity " + entity->name);
}

void Object::DestroyComponent(Component* comp) { 
	// find the object and destroy immediately
	for (auto it = components.begin(); it != components.end(); ++it) {
		if ((*it) == comp) {
			delete comp;
			components.erase(it);
			return;
		}
	}

	DEBUG_WARNING("The component of type " + comp->get_type().get_name() + " was not attached to the object " + name);
}

