#include "EntityFactory.hpp"
#include "Entity.hpp"

// register the index
EntityFactory::EntityFactory(ObjectIndex* index_)
	: index(index_) {
	// allocate some memory
	objects.reserve(10);
}

EntityFactory::~EntityFactory() {
	// deregister the index
	index = nullptr;
	// delete all objects
	for (EC& ec : objects) {
		ec.entity->OnDestroy();
		delete ec.entity;
	}
	objects.clear();
}

void EntityFactory::Update() {
	for (EC& ec : objects) {
		ec.entity->Update();
	}
}

void EntityFactory::LateUpdate() {
	for (EC& ec : objects) {
		ec.entity->LateUpdate();
	}
}

void EntityFactory::Draw() {
	//for (EC& ec : objects) {
	//	ec.entity->Draw();
	//}
}

void EntityFactory::Cleanup() {
	// look for destroyable objects
	for (EC& ec : objects) {
		if (ec.shouldDestroy) {
			ec.entity->OnDestroy();
			delete ec.entity;
		}
	}
}

void EntityFactory::Destroy(Entity* entity) {
	// find the entity and label it destroyed
	for (EC& e : objects) {
		if (e.entity == entity) {
			e.shouldDestroy = true;
			return;
		}
	}
	DEBUG_WARNING("Failed to destroy entity, it could not be found");
}

void EntityFactory::Add(Entity* e) {
	if (!e) {
		DEBUG_ERROR("Failed to initialize the entity and add it! It doesnt exist!");
		return;
	}
	objects.push_back(e);
	e->factory = this;
	e->Start();
}
