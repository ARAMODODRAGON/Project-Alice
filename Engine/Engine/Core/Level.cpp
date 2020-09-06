#include "Level.hpp"

Level::Level(const json& data, LevelManager* levelManager_, ObjectIndex* entityIndex_)
	: levelManager(levelManager_), eFactory(nullptr) {
	// create the entity factory
	eFactory = new EntityFactory(entityIndex_);

	// load data in from the json file
	// TODO
	if (data.contains("updateText") && data["updateText"].is_string())
		updateText = data["updateText"].get<string>();
	if (data.contains("nextLevel") && data["nextLevel"].is_string())
		nextLevel = data["nextLevel"].get<string>();

	printf("%s\n", updateText.c_str());
	
	if (nextLevel.size()) levelManager->LoadLevel(nextLevel);
}

Level::~Level() {
	// delete factory
	if (eFactory) delete eFactory;
	eFactory = nullptr;
	// clear any other values
	levelManager = nullptr;
}

void Level::Update() {
	eFactory->Cleanup();

	eFactory->Update();
}

void Level::Draw() {
	eFactory->LateUpdate();

	eFactory->Draw();
}
