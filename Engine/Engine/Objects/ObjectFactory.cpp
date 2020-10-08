#include "ObjectFactory.hpp"
#include "Object.hpp"
#include "../Core/Level.hpp"

// register the index
ObjectFactory::ObjectFactory()
	: index(nullptr) {
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
}

Object* ObjectFactory::Make(const type typ) {
	// check if it inherits from object
	if (!typ.is_derived_from(type::get<Object>())) {
		DEBUG_ERROR("Type " + typ.get_name() + " is not a type of Object");
		return nullptr;
	}

	// create
	variant obj = typ.get_constructor().invoke();
	if (!obj) {
		DEBUG_ERROR("Could not construct object of type " + typ.get_name());
		return nullptr;
	}

	// add to the factory and return
	Object* object = obj.get_value<Object*>();
	Get()->Add(object);
	return object;
}

Object* ObjectFactory::Make(const string& objectName, const json& instanceData) {
	// if theres no index to search through then the object cant be built
	if (!Get()->index) {
		DEBUG_ERROR("No index available to search through");
		return nullptr;
	}
	// load the data in and confirm that the name matches a type
	json j;
	// if it fails return null
	if (!Get()->index->GetJSON(&j, objectName)) return nullptr;
	string name = j["type"].get<string>();
	type instTy = type::get_by_name(name.c_str());
	if (!instTy.is_valid()) {
		DEBUG_ERROR(name + " is not a valid type");
		return nullptr;
	}

	// if it doesnt derive from Object return null
	if (!instTy.is_derived_from(type::get<Object>())) {
		DEBUG_ERROR("Type " + instTy.get_name() + " is not a type of Object");
		return nullptr;
	}

	// make the object
	variant obj = instTy.create();
	if (!obj.is_valid()) {
		DEBUG_ERROR("failed to create object!");
		return nullptr;
	}

	// load the data into the object
	JsonToObject(instTy, obj, j["data"]);

	// add instance data
	JsonToObject(instTy, obj, instanceData);

	// add it into the factory and return
	Object* object = obj.get_value<Object*>();
	Get()->Add(object);
	return object;
}

Object* ObjectFactory::Make(const type typ, const json& instanceData) {
	// check if it inherits from object
	if (!typ.is_derived_from(type::get<Object>())) {
		DEBUG_ERROR("Type " + typ.get_name() + " is not a type of Object");
		return nullptr;
	}

	// create
	variant obj = typ.get_constructor().invoke();
	if (!obj) {
		DEBUG_ERROR("Could not construct object of type " + typ.get_name());
		return nullptr;
	}

	// add instance data
	JsonToObject(typ, obj, instanceData);

	// add to the factory and return
	Object* object = obj.get_value<Object*>();
	Get()->Add(object);
	return object;
}

void ObjectFactory::Destroy(Object* entity) {
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
	// clear the objects
	Clear();
	// delete the index
	if (index) delete index;
	index = nullptr;
}
