#ifndef _OBJECTS_SCRIPTABLE_OBJECT_FACTORY_HPP
#define _OBJECTS_SCRIPTABLE_OBJECT_FACTORY_HPP
#include "../General/Types.hpp"
#include "../General/Serialization.hpp"
#include "ObjectIndex.hpp"
#include "../Core/Debugger.hpp"
#include "JsonToObjectLoader.hpp"

class ScriptableObject;

class ScriptableObjectFactory {

	static ScriptableObjectFactory* primaryFactory;

	// an scriptable with matching data
	struct SC {
		ScriptableObject* object;
		bool shouldDestroy;

		SC(ScriptableObject* object_) : object(object_), shouldDestroy(false) { }
	};
	// all the entities
	vector<SC> objects;

	// an object index
	ObjectIndex* index;

public:

	ScriptableObjectFactory(ObjectIndex* scriptableIndex);
	~ScriptableObjectFactory();

	// register this as the main instance of a scriptable object factory
	void SetPrimary();
	void UnSetPrimary();
	static ScriptableObjectFactory* GetPrimary() { return primaryFactory; }

	// events
	void Update();
	void LateUpdate();
	void Draw();
	void Cleanup();

	// factory methods
	template<class T> T* Make();
	template<class T> T* Make(const string& objectName);
	void Destroy(ScriptableObject* object);

private:

	// helper function. registers this factory onto the object, adds it into the vector, and calls Start()
	void Add(ScriptableObject* obj);

};

template<class T>
inline T* ScriptableObjectFactory::Make() {
	T* e = new T();
	Add(e);
	return e;
}


template<class T>
inline T* ScriptableObjectFactory::Make(const string& objectName) {
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
	auto givenTy = type::get<T>();
	auto entityTy = type::get<ScriptableObject>();
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
	Add(obj.get_value<ScriptableObject*>());

	// now return the object
	return obj.get_value<T*>();
}

#endif // !_OBJECTS_SCRIPTABLE_OBJECT_FACTORY_HPP