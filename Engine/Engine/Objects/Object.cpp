#include "Object.hpp"
#include "../Core/Level.hpp"
#include "../Physics/PhysicsScene.hpp"

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
	DEBUG_WARNING("The component was not attached to the object " + name);
}

