#include "LevelManager.hpp"

LevelManager::LevelManager(const string& levelFolder, const string& defaultLevel, ObjectIndex* entityIndex_)
	: levelIndex(nullptr)
	, entityIndex(entityIndex_)
	, currentLevel(nullptr)
	, frozenLevel(nullptr)
	, levelToLoad("")
	, levelAction(LevelAction::None) {
	// create a level index based on the given folder
	levelIndex = new ObjectIndex(levelFolder);

	// now load the first level
	json data;
	levelIndex->GetJSON(&data, defaultLevel);
}

LevelManager::~LevelManager() {
	// delete levels
	if (currentLevel) delete currentLevel;
	currentLevel = nullptr;
	if (frozenLevel) delete frozenLevel;
	frozenLevel = nullptr;

	// delete level index
	if (levelIndex) delete levelIndex;
	levelIndex = nullptr;

	// clear any other objects
	entityIndex = nullptr;
}

void LevelManager::Update() {
	currentLevel->Update();
}

void LevelManager::Draw() {
	currentLevel->Draw();
}

void LevelManager::UpdateLevelAction() {
	switch (levelAction) {
		case LevelAction::Replace:
		{
			// unload current
			delete currentLevel;

			// load new level
			json data;
			levelIndex->GetJSON(&data, levelToLoad);
			currentLevel = new Level(data, this, entityIndex);

			break;
		}
		case LevelAction::SwapThenReplace:
		{
			// swap
			Level* tmp = currentLevel;
			currentLevel = frozenLevel;
			frozenLevel = tmp;

			// unload current
			delete currentLevel;

			// load new level
			json data;
			levelIndex->GetJSON(&data, levelToLoad);
			currentLevel = new Level(data, this, entityIndex);

			break;
		}
		case LevelAction::ReplaceFrozen:
		{
			// delete frozen level
			if (frozenLevel) delete frozenLevel;

			// load new level
			json data;
			levelIndex->GetJSON(&data, levelToLoad);
			frozenLevel = new Level(data, this, entityIndex);

			break;
		}
		case LevelAction::Swap:
		{
			// swap
			Level* tmp = currentLevel;
			currentLevel = frozenLevel;
			frozenLevel = tmp;

			break;
		}
		case LevelAction::UnloadAndSwap:
		{
			// unload
			delete currentLevel;

			// swap
			currentLevel = frozenLevel;
			frozenLevel = nullptr;

			break;
		}
		default: break;
	}
}


void LevelManager::LoadLevel(const string& level) {
	if (levelIndex->Contains(level)) {
		levelAction = LevelAction::Replace;
		levelToLoad = level;
	} else {
		DEBUG_ERROR("No level titled " + level + " could be found");
	}
}

void LevelManager::SwapAndLoadLevel(const string& level) {
	if (levelIndex->Contains(level)) {
		levelAction = LevelAction::SwapThenReplace;
		levelToLoad = level;
	} else {
		DEBUG_ERROR("No level titled " + level + " could be found");
	}
}

void LevelManager::LoadFrozen(const string& level) {
	if (levelIndex->Contains(level)) {
		levelAction = LevelAction::ReplaceFrozen;
		levelToLoad = level;
	} else {
		DEBUG_ERROR("No level titled " + level + " could be found");
	}
}

void LevelManager::Swap() {
	if (frozenLevel) {
		levelToLoad = "";
		levelAction = LevelAction::Swap;
	} else {
		DEBUG_ERROR("No frozen level to swap to");
	}
}

void LevelManager::UnloadAndSwap() {
	if (frozenLevel) {
		levelToLoad = "";
		levelAction = LevelAction::UnloadAndSwap;
	} else {
		DEBUG_ERROR("No frozen level to swap to");
	}
}
