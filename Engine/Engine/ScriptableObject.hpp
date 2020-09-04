#ifndef _SCRIPTABLE_OBJECT_HPP
#define _SCRIPTABLE_OBJECT_HPP
#include "General/Types.hpp"
#include "General/Serialization.hpp"

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
	template<class T> static T* Create();
	static void Destroy(const ScriptableObject* scr);
	template<class T> T* FindWithType();

	RTTR_ENABLE()
};

#endif // !_SCRIPTABLE_OBJECT_HPP
