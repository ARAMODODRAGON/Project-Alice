#include "LevelManager.hpp"
#include "Level.hpp"

LevelManager::LevelManager()
	: levelIndex(nullptr)
	, objectIndex(nullptr)
	, currentLevel(nullptr)
	, frozenLevel(nullptr)
	, levelToLoad("")
	, levelAction(LevelAction::None) { }

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
	objectIndex = nullptr;
}

void LevelManager::Init(const string& levelFolder, const string& defaultLevel, FileIndex* objectIndex_) {
	auto* inst = Get();
	// set object index
	inst->objectIndex = objectIndex_;

	// create a level index based on the given folder
	inst->levelIndex = new FileIndex(levelFolder);

	// now load the first level
	if (inst->levelIndex->Contains(defaultLevel))
		inst->MakeLevel(defaultLevel);
}

void LevelManager::Update() {
	auto* inst = Get();
	if (inst->currentLevel)
		inst->currentLevel->Update();
	else {
		DEBUG_ERROR("levelmanager has not been initialized correctly!");
	}
}

void LevelManager::LateUpdate() {
	auto* inst = Get();
	if (inst->currentLevel)
		inst->currentLevel->LateUpdate();
	else {
		DEBUG_ERROR("levelmanager has not been initialized correctly!");
	}
}

void LevelManager::Draw() {
	auto* inst = Get();
	if (inst->currentLevel)
		inst->currentLevel->Draw();
	else {
		DEBUG_ERROR("levelmanager has not been initialized correctly!");
	}
}

void LevelManager::Clean() {
	auto* inst = Get();
	if (inst->currentLevel) {
		// first is a level action
		inst->DoLevelAction();

		// now clean level
		inst->currentLevel->Clean();
	} else {
		DEBUG_ERROR("levelmanager has not been initialized correctly!");
	}
}

void LevelManager::Exit() {
	auto* inst = Get();
	if (inst->currentLevel) {
		inst->currentLevel->Exit();
		if (inst->frozenLevel)
			inst->frozenLevel->Exit();
	}
}

void LevelManager::DoLevelAction() {
	switch (levelAction) {
		case LevelAction::Replace:
		{
			// check
			if (!levelIndex->Contains(levelToLoad)) {
				DEBUG_ERROR("No level titled " + levelToLoad + " could be found");
				// reset
				ResetActions();
				return;
			}

			// unload current
			delete currentLevel;

			// load new level
			MakeLevel(levelToLoad);

			// reset
			ResetActions();
			break;
		}
		case LevelAction::SwapThenReplace:
		{
			// check
			if (!levelIndex->Contains(levelToLoad)) {
				DEBUG_ERROR("No level titled " + levelToLoad + " could be found");
				// reset
				ResetActions();
				return;
			}

			// swap
			Level* tmp = currentLevel;
			currentLevel = frozenLevel;
			frozenLevel = tmp;

			// unload current
			delete currentLevel;

			// load new level 
			MakeLevel(levelToLoad);

			// reset
			ResetActions();
			break;
		}
		case LevelAction::ReplaceFrozen:
		{
			// check
			if (!levelIndex->Contains(levelToLoad)) {
				DEBUG_ERROR("No level titled " + levelToLoad + " could be found");
				// reset
				ResetActions();
				return;
			}

			// delete frozen level
			if (frozenLevel) delete frozenLevel;
			frozenLevel = nullptr;

			// load new level 
			MakeLevel(levelToLoad, false);

			// reset
			ResetActions();
			break;
		}
		case LevelAction::DeleteFrozen:
		{
			// check 
			if (!frozenLevel) {
				DEBUG_WARNING("No frozen level currently loaded");
				// reset
				ResetActions();
				return;
			}

			// delete
			delete frozenLevel; // check was made before reaching this case here
			frozenLevel = nullptr;

			// reset
			ResetActions();
			break;
		}
		case LevelAction::Swap:
		{
			// check 
			if (!frozenLevel) {
				DEBUG_WARNING("No frozen level currently loaded");
				// reset
				ResetActions();
				return;
			}

			// swap
			Level* tmp = currentLevel;
			currentLevel = frozenLevel;
			frozenLevel = tmp;

			// reset
			ResetActions();
			break;
		}
		case LevelAction::UnloadAndSwap:
		{
			// check 
			if (!frozenLevel) {
				DEBUG_WARNING("No frozen level currently loaded");
				// reset
				ResetActions();
				return;
			}

			// unload
			delete currentLevel;

			// swap
			currentLevel = frozenLevel;
			frozenLevel = nullptr;

			// reset
			ResetActions();
			break;
		}
		default: break;
	}
}

#pragma region Level Loading

void LevelManager::LoadLevel(const string& level) {
	Get()->levelAction = LevelAction::Replace;
	Get()->levelToLoad = level;
}

void LevelManager::SwapAndLoadLevel(const string& level) {
	Get()->levelAction = LevelAction::SwapThenReplace;
	Get()->levelToLoad = level;
}

void LevelManager::LoadFrozen(const string& level) {
	Get()->levelAction = LevelAction::ReplaceFrozen;
	Get()->levelToLoad = level;
}

void LevelManager::UnloadFrozen() {
	Get()->levelAction = LevelAction::DeleteFrozen;
	Get()->levelToLoad = "";
}

void LevelManager::Swap() {
	Get()->levelAction = LevelAction::Swap;
	Get()->levelToLoad = "";
}

void LevelManager::UnloadAndSwap() {
	Get()->levelAction = LevelAction::UnloadAndSwap;
	Get()->levelToLoad = "";
}

void LevelManager::ResetActions() {
	levelAction = LevelAction::None;
	levelToLoad = "";
}

#pragma endregion

void LevelManager::MakeLevel(const string& levelToLoad_, bool loadAsCurrent) {
	// load data
	json data;
	levelIndex->GetJSON(&data, levelToLoad_);

	// create level
	if (loadAsCurrent) {
		currentLevel = new Level(objectIndex);
		currentLevel->Init(data);
		DEBUG_LOG("Loaded level \"" + levelToLoad_ + "\" as primary");
	} else {
		frozenLevel = new Level(objectIndex);
		frozenLevel->Init(data);
		DEBUG_LOG("Loaded level \"" + levelToLoad_ + "\" as secondary");
	}


}
