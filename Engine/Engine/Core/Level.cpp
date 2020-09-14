#include "Level.hpp"
#include "../Objects/Object.hpp"

Level::Level(const json& data, LevelManager* levelManager_, FileIndex* objectIndex_, ContentHandler* content_) // TODO
	: levelManager(levelManager_), objFactory(nullptr), renderScene(nullptr) {
	// initialize the render scene
	renderScene = new RenderScene(content_);

	// create the entity factory
	objFactory = new ObjectFactory(this, objectIndex_);

	if (data.contains("Objects") && data["Objects"].is_array())
		LoadObjects(data["Objects"]);
}

Level::~Level() {
	// delete factory
	if (objFactory) delete objFactory;
	objFactory = nullptr;
	// delete render scene
	if (renderScene) delete renderScene;
	renderScene = nullptr;
	// clear any other values
	levelManager = nullptr;
}

void Level::Update() {
	objFactory->Cleanup();

	objFactory->Update();
}

void Level::Draw() {
	objFactory->LateUpdate();

	renderScene->Draw();
}

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
