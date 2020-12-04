#ifndef _OBJECTS_COMPONENT_HPP
#define _OBJECTS_COMPONENT_HPP

// lightweight class that attaches to an object and can have different properties
// such examples of components are Sprites, Colliders, Cameras
class Component {

	// base properties
	friend class Object;
	Object* object; // the object that this component is attached to
	bool isActive;

public:

	Component();
	virtual ~Component() = 0 { } // abstract

	// virtual functions
	virtual void Start() { }
	virtual void OnDestroy() { }

	// getters & setters
	Object* GetObject() const { return object; }

	bool GetIsActive() const { return isActive; }
	void SetIsActive(bool isActive_) { isActive = isActive_; }

};

#endif // !_OBJECTS_COMPONENT_HPP