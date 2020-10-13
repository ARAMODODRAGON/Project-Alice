#include "Object.hpp"
#include "../Core/Level.hpp"
#include "../Physics/PhysicsScene.hpp"

RTTR_REGISTRATION {
	registration::class_<Object>("Object")
		.public_object_constructor
		.property("isActive", &Object::isActive)
		.property("name", &Object::name)
		.property("position", &Object::position)
		.property("velocity", &Object::velocity)
		.property("rotation", &Object::rotation)
		.property("components", &Object::components);
}

// TODO: add VAO code
Object::Object()
	: level(nullptr)
	, isActive(true)
	, name("Object")
	, position(0.0f)
	, velocity(0.0f)
	, rotation(0.0f) {
	level = LevelManager::GetLevel();
	PhysicsScene::AddObject(this);
}

Object::~Object() {
	// destroy the components
	for (Component* c : components) {
		c->OnDestroy();
		delete c;
	}
	components.clear();
	PhysicsScene::RemoveObject(this);
}

Object* Object::Make() {
	return ObjectFactory::Make<Object>();

}

Object* Object::Make(const string& objectName) {
	return ObjectFactory::Make<Object>(objectName);
}

void Object::Destroy(Object* entity) {
	return ObjectFactory::Destroy(entity);
}

void Object::DestroyComponent(Component* comp) {
	// find the object and destroy immediately
	for (auto it = components.begin(); it != components.end(); ++it) {
		if ((*it) == comp) {
			comp->OnDestroy();
			delete comp;
			components.erase(it);
			return;
		}
	}

	DEBUG_WARNING("The component of type " + comp->get_type().get_name() + " was not attached to the object " + name);
}

