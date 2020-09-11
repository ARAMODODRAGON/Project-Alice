#ifndef _OBJECTS_COMPONENT_HPP
#define _OBJECTS_COMPONENT_HPP
#include "../General/Serialization.hpp"

// lightweight class that attaches to an object and can have different properties
// such examples of components are Sprites, Colliders, Cameras
class Component {

	// base properties
	class Object* object; // the object that this component is attached to
	bool isActive;

public:

	Component();
	virtual ~Component() = 0 { }; // abstract

	// getters & setters
	Object* GetObject() const { return object; }

	bool GetIsActive() const { return isActive; }
	void SetIsActive(bool isActive_) { isActive = isActive_; }

	RTTR_ENABLE() RTTR_REGISTRATION_FRIEND
};

#endif // !_OBJECTS_COMPONENT_HPP