#include "ObjectFactory.hpp"
#include "Object.hpp"

// register the index
ObjectFactory::ObjectFactory(FileIndex* index_)
	: index(index_) {
	// allocate some memory
	objects.reserve(10);
}

ObjectFactory::~ObjectFactory() {
	// deregister the index
	index = nullptr;
	// delete all objects
	for (PairType& ec : objects) {
		ec.first->OnDestroy();
		delete ec.first;
	}
	objects.clear();
}

void ObjectFactory::Update() {
	for (PairType& ec : objects) {
		if (ec.second || !ec.first->GetIsActive()) continue;
		ec.first->Update();
	}
}
void ObjectFactory::LateUpdate() {
	for (PairType& ec : objects) {
		if (ec.second || !ec.first->GetIsActive()) continue;
		ec.first->LateUpdate();
	}
}

void ObjectFactory::Cleanup() {
	// look for destroyable objects
	for (auto it = objects.begin(); it != objects.end(); ++it) {
		if (it->second) {
			it->first->OnDestroy();
			delete it->first;
			it = objects.erase(it);
		}
	}
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
	Add(object);
	return object;
}

Object* ObjectFactory::Make(const string& objectName, const json& instanceData) {
	// if theres no index to search through then the object cant be built
	if (!index) {
		DEBUG_ERROR("No index available to search through");
		return nullptr;
	}
	// load the data in and confirm that the name matches a type
	json j;
	// if it fails return null
	if (!index->GetJSON(&j, objectName)) return nullptr;
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
	Add(object);
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
	Add(object);
	return object;
}

void ObjectFactory::Destroy(Object* entity) {
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
	objects.push_back(PairType(e, false));
	
	// add the factory pointer
	//type::get<Object>().get_property("factory").set_value(e, this);
	e->factory = this;

	// call start
	e->Start();
}
