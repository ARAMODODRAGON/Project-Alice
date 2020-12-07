#include "ObjectFactory.hpp"
#include "Object.hpp"
#include "../Core/Level.hpp"

// register the index
ObjectFactory::ObjectFactory()
	: index(nullptr), disableDestroy(false) {
	// allocate some memory
	objects.reserve(20);
}

ObjectFactory::~ObjectFactory() { DestroyAll(); }

void ObjectFactory::Init(const string& indexPath) {
	Get()->index = new FileIndex(indexPath);
}

void ObjectFactory::Update() {
	// get reference
	auto& objects = Get()->objects;

	for (auto it = objects.begin(); it != objects.end(); ++it) {
		if (it->second || !it->first->GetIsActive()) continue;
		it->first->Update();
	}

}
void ObjectFactory::LateUpdate() {
	// get reference
	auto& objects = Get()->objects;

	for (auto it = objects.begin(); it != objects.end(); ++it) {
		if (it->second || !it->first->GetIsActive()) continue;
		it->first->LateUpdate();
	}
}

void ObjectFactory::Cleanup() {
	// get reference
	auto& objects = Get()->objects;
	auto& newObjects = Get()->newObjects;

	// add all new objects
	objects.reserve(objects.size() + newObjects.size());
	for (Object* no : newObjects) {
		objects.push_back(PairType(no, false));
	}
	newObjects.clear();

	// look for destroyable objects
	for (auto it = objects.begin(); it != objects.end(); ++it) {
		if (it->second) {
			it->first->OnDestroy();
			delete it->first;
			it = objects.erase(it);
			// if erase returns an iterator at the end of the array then it quits
			if (it == objects.end()) break;
		}
	}
}

void ObjectFactory::Exit() { Get()->DestroyAll(); }

void ObjectFactory::Clear() { 
	Get()->disableDestroy = true;
	// get ref
	auto& objects = Get()->objects;
	auto& newObjects = Get()->newObjects;

	// delete all objects
	for (PairType& ec : objects) {
		ec.first->OnDestroy();
		delete ec.first;
	}
	objects.clear();
	for (Object* no : newObjects) {
		delete no;
	}
	newObjects.clear();

	Get()->disableDestroy = false;
}

void ObjectFactory::Destroy(Object* entity) {
	if (Get()->disableDestroy) return;
	// get reference
	auto& objects = Get()->objects;

	// find the entity and label it destroyed
	for (PairType& e : objects) {
		if (e.first == entity) {
			e.second = true;
			return;
		}
	}
	DEBUG_WARNING("Failed to destroy entity, it could not be found");
}

void ObjectFactory::Add(Object* e) {
	if (!e) {
		DEBUG_ERROR("Failed to initialize the entity and add it! It doesnt exist!");
		return;
	}
	newObjects.push_back(e);
	
	// call start
	e->Start();
}

void ObjectFactory::DestroyAll() {
	disableDestroy = true;
	// clear the objects
	Clear();
	// delete the index
	if (index) delete index;
	index = nullptr;
	disableDestroy = false;
}
