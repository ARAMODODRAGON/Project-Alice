#ifndef _SCRIPTABLE_OBJECT_HPP
#define _SCRIPTABLE_OBJECT_HPP
#include "GeneralTypes.hpp"
#include "RTTR.hpp"

class ScriptableObject {
public:

	// constructor & destructor
	ScriptableObject();
	virtual ~ScriptableObject() = 0;

	// events
	virtual void Start() { }
	virtual void Update() { }
	virtual void LateUpdate() { }
	virtual void OnDestroy() { }

	// static functions
	template<class T> static T* Create() { }
	template<class T> static T* Create(const string& filename) { }
	static void Destroy(const ScriptableObject* scr) { }

	RTTR_ENABLE() 
};

#endif // !_SCRIPTABLE_OBJECT_HPP