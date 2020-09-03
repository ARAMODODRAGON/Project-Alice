#ifndef _SCRIPTABLE_OBJECT_HPP
#define _SCRIPTABLE_OBJECT_HPP
#include "GeneralTypes.hpp"

class ScriptableObject {

public:

	// events
	virtual void Start() { }
	virtual void Update() { }
	virtual void LateUpdate() { }
	virtual void OnDestroy() { }

	// static functions
	template<class T> static T* Create();
	template<class T> static T* Create(const string& filename);
	static void Destroy(const ScriptableObject* scr);

};

void ScriptableObject::Destroy(const ScriptableObject* scr) { 

}

template<class T>
inline T* ScriptableObject::Create() {
	return nullptr;
}

template<class T>
inline T* ScriptableObject::Create(const string& filename) {
	return nullptr;
}

#endif // !_SCRIPTABLE_OBJECT_HPP