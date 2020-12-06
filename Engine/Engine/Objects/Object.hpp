#ifndef _ENTITY_HPP
#define _ENTITY_HPP
#include "../General/Types.hpp"
#include "ObjectFactory.hpp"
#include "Component.hpp"
#include "../Physics/Colliders.hpp"

class Level;

class Object {
	friend ObjectFactory;

	// level
	Level* level;

	// base properties
	bool isActive;
	string name;
	vec2 position;
	vec2 velocity;
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

	// collision callbacks
	virtual void OnCollisionEnter(const CollisionData& col) { }
	virtual void OnCollisionStay(const CollisionData& col) { }
	virtual void OnCollisionExit(const CollisionData& col) { }

	// component creation / destruction
	template<class T> T* AddComponent();
	void DestroyComponent(Component* comp);
	template<class T> T* GetComponent();

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

	float GetRotation() const { return rotation; }
	void SetRotation(const float& rotation_) { rotation = rotation_; }

};

// component creation / destruction

template<class T>
inline T* Object::AddComponent() {
	// create and add to list
	T* comp = new T();
	components.push_back(comp);

	// add object reference
	Component* c = comp;
	c->object = this;
	c->Start();

	// return
	return comp;
}

template<class T>
inline T* Object::GetComponent() {
	for (Component* c : components) {
		if (c->get_type() == type::get<T>()) {
			return rttr_cast<T*>(c);
		}
	}
	DEBUG_WARNING("No component was found with type " + type::get<T>().get_name());
	return nullptr;
}

#endif // !_ENTITY_HPP