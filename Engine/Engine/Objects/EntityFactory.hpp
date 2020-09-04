#ifndef _CORE_ENTITY_FACTORY_HPP
#define _CORE_ENTITY_FACTORY_HPP
#include "../General/Types.hpp"
#include "../General/Serialization.hpp"
#include "ObjectIndex.hpp"
#include "../Core/Debugger.hpp"

class Entity;

class EntityFactory {

	// an entity with matching data
	struct EC {
		Entity* entity;
		bool shouldDestroy;

		EC(Entity* entity_) : entity(entity_), shouldDestroy(false) { }
	};
	// all the entities
	vector<EC> objects;

	// an object index
	ObjectIndex* index;

public:

	EntityFactory(ObjectIndex* index_ = nullptr);
	~EntityFactory();

	// events
	void Update();
	void LateUpdate();
	void Draw();
	void Cleanup();

	// factory methods
	template<class T> T* Make();
	template<class T> T* Make(const string& objectName);
	//Entity* Make(const string& objectName);
	void Destroy(Entity* entity);

private:

	// helper function. registers this factory onto the entity, adds it into the vector, and calls Start()
	void Add(Entity* e);

};


template<class T>
inline T* EntityFactory::Make() {
	T* e = new T();
	Add(e);
	return e;
}


template<class T>
inline T* EntityFactory::Make(const string& objectName) {
	// if theres no index to search through then the object cant be built
	if (!index) {
		DEBUG_ERROR("No index available to search through");
		return nullptr;
	}
	// load the data in and confirm that the name matches a type
	json j;
	// if it fails return null
	if (!index->GetJSON(&j, objectName))	return nullptr;
	string name = j["type"].get<string>();
	auto objectTy = type::get_by_name(name.c_str());
	if (!objectTy.is_valid()) {
		DEBUG_ERROR(name + " is not a valid type");
		return nullptr;
	}

	// now confirm that the class exists and can convert to type T
	auto givenTy = type::get<T>();
	auto entityTy = type::get<Entity>();
	// if it doesnt derive from T and entity return null
	if (!objectTy.is_derived_from(givenTy) && objectTy.is_derived_from(entityTy))
		return nullptr;

	// make the object
	variant obj = objectTy.create();
	if (!obj.is_valid()) {
		DEBUG_ERROR("failed to create object!");
		return nullptr;
	}

	// load the data into the object
	// TODO: load the data

	// add it into the factory
	Add(obj.get_value<Entity*>());

	// now return the object
	return obj.get_value<T*>();
}


#endif // !_CORE_ENTITY_FACTORY_HPP
