#ifndef _CORE_ENTITY_FACTORY_HPP
#define _CORE_ENTITY_FACTORY_HPP
#include "../General/Types.hpp"
#include "../General/Macros.hpp"
#include "../General/Serialization.hpp"
#include "FileIndex.hpp"
#include "../Core/Debugger.hpp"
#include "JsonToObjectLoader.hpp"

class Object;
class ILevel;

class ObjectFactory {
	PRIVATE_SINGLETON(ObjectFactory);

	// an object with a bool. this bool is whether or not the object should destroy
	using PairType = pair<Object*, bool>;
	// all the entities
	vector<PairType> objects; // TODO: seperate array for new objects, these then get pushed into this array during cleanup

	// an object index & level
	FileIndex* index;

public:

	ObjectFactory();
	~ObjectFactory();

	// events
	static void Init(const string& indexPath);
	static void Update();
	static void LateUpdate();
	static void Cleanup();
	static void Exit();
	static void Clear();

	// factory methods
	template<class T> static T* Make();
	template<class T> static T* Make(const string& objectName);
	static Object* Make(const type typ);
	static Object* Make(const string& objectName, const json& instanceData);
	static Object* Make(const type typ, const json& instanceData);
	static void Destroy(Object* entity);

private:

	// helper functions
	void Add(Object* e);
	void DestroyAll();

};


template<class T>
inline T* ObjectFactory::Make() {
	T* e = new T();
	Get()->Add(e);
	return e;
}


template<class T>
inline T* ObjectFactory::Make(const string& objectName) {
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
	Get()->Add(obj.get_value<Object*>());

	// now return the object
	return obj.get_value<T*>();
}


#endif // !_CORE_ENTITY_FACTORY_HPP
