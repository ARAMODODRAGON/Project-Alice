#include "Level.hpp"
#include "../Objects/Object.hpp"

Level::Level(FileIndex* objectIndex_)
	: objFactory(nullptr), renderScene(nullptr) {
	// initialize the render scene
	renderScene = new RenderScene();

	// create the entity factory
	objFactory = new ObjectFactory(this, objectIndex_);

}

Level::~Level() {
	// delete factory
	if (objFactory) delete objFactory;
	objFactory = nullptr;
	// delete render scene
	if (renderScene) delete renderScene;
	renderScene = nullptr;
}

void Level::Init(const json& data) {
	if (data.contains("Objects") && data["Objects"].is_array())
		LoadObjects(data["Objects"]);
}

void Level::Update() {
	objFactory->Cleanup();

	objFactory->Update();
}

void Level::LateUpdate() {
	objFactory->LateUpdate();
}

void Level::Draw() {
	renderScene->Draw();
}

void Level::Clean() { objFactory->Cleanup(); }

void Level::Exit() { }

void Level::LoadObjects(const json& data) {
	for (auto it = data.begin(); it != data.end(); ++it) {
		if (!it->is_object()) {
			DEBUG_ERROR("Non object in object array! Skipped");
			continue;
		}
		const json& objData = (*it);

		// construct using index
		if (objData.contains("objectName") && objData["objectName"].is_string()) {
			string objectName = objData["objectName"].get<string>();

			// construct with instance data
			if (objData.contains("data") && objData.is_object())
				objFactory->Make(objectName, objData["data"]);

			// construct without instance data
			else objFactory->Make<Object>(objectName);
		}
		// construct using type
		else if (objData.contains("type") && objData["type"].is_string()) {
			string typName = objData["type"].get<string>();
			type typ = type::get_by_name(typName.c_str());

			// construct with instance data
			if (objData.contains("data") && objData.is_object())
				objFactory->Make(typ, objData["data"]);

			// construct without instance data
			else objFactory->Make(typ);
		}
		// if nothing print error
		else {
			DEBUG_ERROR("No Type or ObjectName found");
		}

	}
}
