#include "LevelManager.hpp"
#include "Level.hpp"
#include "../Game.hpp"

LevelManager::LevelManager()
	: currentLevel(nullptr)
	, levelToLoad("")
	, levelAction(LevelAction::None)
	, index(nullptr) { }

LevelManager::~LevelManager() {
	// delete level
	if (currentLevel) delete currentLevel;
	currentLevel = nullptr;
	// delete index
	if (index) delete index;
	index = nullptr;
}

void LevelManager::Init(const string& levelFolder, const string& defaultLevel) {
	// load the index
	if (Get()->index == nullptr)
		Get()->index = new FileIndex(levelFolder);

	// load the first level
	LoadLevel(defaultLevel);
	Get()->DoLevelAction();
}

void LevelManager::Update() {
	auto* inst = Get();
	#if _DEBUG
	if (inst->currentLevel)
		inst->currentLevel->Update();
	else {
		DEBUG_ERROR("levelmanager has not been initialized correctly!");
	}
	#else
	inst->currentLevel->Update();
	#endif
}

void LevelManager::LateUpdate() {
	auto* inst = Get();
	#if _DEBUG
	if (inst->currentLevel)
		inst->currentLevel->LateUpdate();
	else {
		DEBUG_ERROR("levelmanager has not been initialized correctly!");
	}
	#else
	inst->currentLevel->LateUpdate();
	#endif
}

void LevelManager::Cleanup() {
	Get()->DoLevelAction();
}

void LevelManager::Exit() {
	auto* inst = Get();
	if (inst->currentLevel) {
		inst->currentLevel->Exit();
	}
}

void LevelManager::DoLevelAction() {
	switch (levelAction) {
		case LevelAction::Replace:
		{
			// unload current
			if (currentLevel) {
				currentLevel->Exit();
				delete currentLevel;
			}

			// load new level
			MakeLevel();

			// reset
			ResetActions();
			break;
		}
		//case LevelAction::SwapThenReplace:
		//{
		//	// check
		//	if (!levelIndex->Contains(levelToLoad)) {
		//		DEBUG_ERROR("No level titled " + levelToLoad + " could be found");
		//		// reset
		//		ResetActions();
		//		return;
		//	}
		//
		//	// swap
		//	Level* tmp = currentLevel;
		//	currentLevel = frozenLevel;
		//	frozenLevel = tmp;
		//
		//	// unload current
		//	delete currentLevel;
		//
		//	// load new level 
		//	MakeLevel(levelToLoad);
		//
		//	// reset
		//	ResetActions();
		//	break;
		//}
		//case LevelAction::ReplaceFrozen:
		//{
		//	// check
		//	if (!levelIndex->Contains(levelToLoad)) {
		//		DEBUG_ERROR("No level titled " + levelToLoad + " could be found");
		//		// reset
		//		ResetActions();
		//		return;
		//	}
		//
		//	// delete frozen level
		//	if (frozenLevel) delete frozenLevel;
		//	frozenLevel = nullptr;
		//
		//	// load new level 
		//	MakeLevel(levelToLoad, false);
		//
		//	// reset
		//	ResetActions();
		//	break;
		//}
		//case LevelAction::DeleteFrozen:
		//{
		//	// check 
		//	if (!frozenLevel) {
		//		DEBUG_WARNING("No frozen level currently loaded");
		//		// reset
		//		ResetActions();
		//		return;
		//	}
		//
		//	// delete
		//	delete frozenLevel; // check was made before reaching this case here
		//	frozenLevel = nullptr;
		//
		//	// reset
		//	ResetActions();
		//	break;
		//}
		//case LevelAction::Swap:
		//{
		//	// check 
		//	if (!frozenLevel) {
		//		DEBUG_WARNING("No frozen level currently loaded");
		//		// reset
		//		ResetActions();
		//		return;
		//	}
		//
		//	// swap
		//	Level* tmp = currentLevel;
		//	currentLevel = frozenLevel;
		//	frozenLevel = tmp;
		//
		//	// reset
		//	ResetActions();
		//	break;
		//}
		//case LevelAction::UnloadAndSwap:
		//{
		//	// check 
		//	if (!frozenLevel) {
		//		DEBUG_WARNING("No frozen level currently loaded");
		//		// reset
		//		ResetActions();
		//		return;
		//	}
		//
		//	// unload
		//	delete currentLevel;
		//
		//	// swap
		//	currentLevel = frozenLevel;
		//	frozenLevel = nullptr;
		//
		//	// reset
		//	ResetActions();
		//	break;
		//}
		default: break;
	}
}

void LevelManager::LoadLevel(const string& level) {


	Get()->levelAction = LevelAction::Replace;
	Get()->levelToLoad = level;

}

void LevelManager::ResetActions() {
	levelAction = LevelAction::None;
	levelToLoad = "";
}

void LevelManager::MakeLevel() {
	// check
	if (!index) {
		DEBUG_ERROR("No level index is loaded! Failed to load level!");
		return;
	}
	if (!index->Contains(levelToLoad)) {
		DEBUG_ERROR("No level with name " + levelToLoad + "! Failed to load level!");
		return;
	}

	// get data
	json j = index->GetJSON(levelToLoad);

	if (!j.contains("@type")) {
		DEBUG_ERROR("Invalid level file. No type specified");
		return;
	}

	// get level type
	string levelTyS = j["@type"];
	type levelTy = type::get_by_name(levelTyS.c_str());

	if (!levelTy) {
		DEBUG_ERROR("Could not load level of type " + levelTyS);
		return;
	}

	variant lvl = levelTy.create();
	Level* level = lvl.get_value<Level*>();

	if (!level) {
		DEBUG_ERROR("Failed to convert level to pointer! If you see this contact Dom.");
		return;
	}

	// set the level
	currentLevel = level;

	// if everything is successful then load the level data
	Game::Get()->LevelLoad(level, j);

	// finally init 
	currentLevel->Init();
	DEBUG_LOG("Loaded level \"" + levelToLoad + "\" successfully!");
}
