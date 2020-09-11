#ifndef _ENTITY_HPP
#define _ENTITY_HPP
#include "../General/Types.hpp"
#include "../General/Serialization.hpp"
#include "ObjectFactory.hpp"
#include "Component.hpp"

class Level;

class Object {

	// factory
	friend ObjectFactory;
	ObjectFactory* factory; // the factory that this entity is attached to

	// level
	Level* level;

	// base properties
	bool isActive;
	string name;
	vec2 position;
	vec2 velocity;
	vec2 scale;
	float rotation;

	// components
	vector<Component*> components;

public:

	// constructor & destructor
	Object();
	virtual ~Object();

	// events
	virtual void Start() { }
	virtual void Update() { }
	virtual void LateUpdate() { }
	virtual void OnDestroy() { }

	// object creation / destruction
	template<class T> T* Make();
	Object* Make();
	template<class T> T* Make(const string& objectName);
	Object* Make(const string& objectName);
	static void Destroy(Object* entity);

	// component creation / destruction
	template<class T> T* AddComponent();
	void DestroyComponent(Component* comp);
	template<class T> T* FindComponentWithType();

	// getters & setters
	Level* GetLevel() const { return level; }

	bool GetIsActive() const { return isActive; }
	void SetIsActive(bool isActive_) { isActive = isActive_; }

	const string& GetName() const { return name; }
	void SetName(const string& name_) { name = name_; }

	vec2 GetPosition() const { return position; }
	void SetPosition(const vec2& position_) { position = position_; }

	vec2 GetVelocity() const { return velocity; }
	void SetVelocity(const vec2& velocity_) { velocity = velocity_; }

	vec2 GetScale() const { return scale; }
	void SetScale(const vec2& scale_) { scale = scale_; }

	float GetRotation() const { return rotation; }
	void SetRotation(const float& rotation_) { rotation = rotation_; }

	RTTR_ENABLE() RTTR_REGISTRATION_FRIEND
};

template<class T>
inline T* Object::Make() {
	if (factory)
		return factory->Make<T>();
	else {
		DEBUG_ERROR("No factory attached to the entity " + name);
		return nullptr;
	}
}

template<class T>
inline T* Object::Make(const string& objectName) {
	if (factory)
		return factory->Make<T>(objectName);
	else {
		DEBUG_ERROR("No factory attached to the entity " + name);
		return nullptr;
	}
}

// component creation / destruction

template<class T>
inline T* Object::AddComponent() {
	// create and add to list
	T* comp = new T();
	components.push_back(comp);

	// add object reference
	type::get<Component>().get_property("object").set_value(comp, this);

	// return
	return comp;
}

template<class T>
inline T* Object::FindComponentWithType() {
	for (Component* c : components) {
		if (c->get_type() == type::get<T>()) {
			return rttr_cast<T*>(c);
		}
	}
	DEBUG_WARNING("No component was found with type " + c->get_type().get_name());
	return nullptr;
}

#endif // !_ENTITY_HPP