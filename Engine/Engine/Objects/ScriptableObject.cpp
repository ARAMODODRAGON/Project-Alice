#include "ScriptableObject.hpp"

RTTR_REGISTRATION {
	registration::class_<ScriptableObject>("ScriptableObject")
		.property("name", &ScriptableObject::name);
}

ScriptableObject::ScriptableObject() : name("Scriptable") { }

ScriptableObject::~ScriptableObject() {

}

void ScriptableObject::Destroy(ScriptableObject* scr) {
	if (scr->factory) scr->factory->Destroy(scr);
	else DEBUG_ERROR("No attached factory");
}
