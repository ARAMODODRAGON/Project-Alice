#ifndef _SCRIPTABLE_OBJECT_HPP
#define _SCRIPTABLE_OBJECT_HPP
#include "ScriptableObjectFactory.hpp"

class ScriptableObject {

	friend ScriptableObjectFactory;
	ScriptableObjectFactory* factory;

	string name;

public:

	// constructor & destructor
	ScriptableObject();
	virtual ~ScriptableObject() = 0;

	// events
	virtual void Start() { }
	virtual void Update() { }
	virtual void LateUpdate() { }
	virtual void Draw() { }
	virtual void OnDestroy() { }

	// getters & setters
	string GetName() const { return name; }

	// creation & destruction
	template<class T> T* Make();
	template<class T> T* Make(const string& objectName);
	static void Destroy(ScriptableObject* scr);

	RTTR_ENABLE() RTTR_REGISTRATION_FRIEND
};

#endif // !_SCRIPTABLE_OBJECT_HPP

// creation & destruction

template<class T>
inline T* ScriptableObject::Make() {
	if (factory)
		return factory->Make<T>();
	else {
		DEBUG_ERROR("No attached factory");
		return nullptr;
	}
}

template<class T>
inline T* ScriptableObject::Make(const string& objectName) {
	if (factory)
		return factory->Make<T>(objectName);
	else {
		DEBUG_ERROR("No attached factory");
		return nullptr;
	}
}
