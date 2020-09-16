#ifndef _CORE_ENTITY_FACTORY_HPP
#define _CORE_ENTITY_FACTORY_HPP
#include "../General/Types.hpp"
#include "../General/Serialization.hpp"
#include "FileIndex.hpp"
#include "../Core/Debugger.hpp"
#include "JsonToObjectLoader.hpp"

class Object;
class Level;

class ObjectFactory {

	// an object with a bool. this bool is whether or not the object should destroy
	using PairType = pair<Object*, bool>;
	// all the entities
	vector<PairType> objects; // TODO: seperate array for new objects, these then get pushed into this array during cleanup

	// an object index & level
	Level* level;
	FileIndex* index;

public:

	ObjectFactory(Level* level_, FileIndex* index_ = nullptr);
	~ObjectFactory();

	// events
	void Update();
	void LateUpdate();
	void Cleanup();

	// factory methods
	template<class T> T* Make();
	template<class T> T* Make(const string& objectName);
	Object* Make(const type typ);
	Object* Make(const string& objectName, const json& instanceData);
	Object* Make(const type typ, const json& instanceData);
	void Destroy(Object* entity);

private:

	// helper function. registers this factory onto the entity, adds it into the vector, and calls Start()
	void Add(Object* e);

};


template<class T>
inline T* ObjectFactory::Make() {
	T* e = new T();
	Add(e);
	return e;
}


template<class T>
inline T* ObjectFactory::Make(const string& objectName) {
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
	auto objectTy = type::get_by_name(name.c_str());
	if (!objectTy.is_valid()) {
		DEBUG_ERROR(name + " is not a valid type");
		return nullptr;
	}

	// now confirm that the class exists and can convert to type T
	type givenTy = type::get<T>();
	type entityTy = type::get<Object>();
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
	JsonToObject(objectTy, obj, j["data"]);

	// add it into the factory
	Add(obj.get_value<Object*>());

	// now return the object
	return obj.get_value<T*>();
}


#endif // !_CORE_ENTITY_FACTORY_HPP
