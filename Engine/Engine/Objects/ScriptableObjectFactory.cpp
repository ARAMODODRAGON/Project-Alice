#include "ScriptableObjectFactory.hpp"
#include "ScriptableObject.hpp"

ScriptableObjectFactory* ScriptableObjectFactory::primaryFactory = nullptr;

ScriptableObjectFactory::ScriptableObjectFactory(ObjectIndex* scriptableIndex)
	: index(scriptableIndex) {
	// reserve some space
	objects.reserve(10);
}

ScriptableObjectFactory::~ScriptableObjectFactory() {
	// remove this if its the primary instance
	UnSetPrimary();
}

void ScriptableObjectFactory::SetPrimary() {
	if (primaryFactory && primaryFactory != this)
		primaryFactory->UnSetPrimary();
	primaryFactory = this;
}

void ScriptableObjectFactory::UnSetPrimary() {
	if (primaryFactory == this) primaryFactory = nullptr;
}

void ScriptableObjectFactory::Update() {
	for (SC& s : objects) {
		s.object->Update();
	}
}
void ScriptableObjectFactory::LateUpdate() {
	for (SC& s : objects) {
		s.object->LateUpdate();
	}
}
void ScriptableObjectFactory::Draw() {
	for (SC& s : objects) {
		s.object->Draw();
	}
}

void ScriptableObjectFactory::Cleanup() { 
	// look for a destroyable and then destroy it
	for (auto it = objects.begin(); it != objects.end(); ++it) {
		if (it->shouldDestroy) {
			it->object->OnDestroy();
			delete it->object;
			it = objects.erase(it);
		}
	}
}

void ScriptableObjectFactory::Destroy(ScriptableObject* object) { 
	for (SC& s : objects) {
		if (s.object == object) {
			s.shouldDestroy = true;
			return;
		}
	}
	DEBUG_WARNING("Could not find scriptable object to destroy");
}

void ScriptableObjectFactory::Add(ScriptableObject* obj) { 
	if (!obj) {
		DEBUG_ERROR("Failed to initialize the scriptable object and add it! It doesnt exist!");
		return;
	}
	objects.push_back(obj);
	obj->factory = this;
	obj->Start();
}
