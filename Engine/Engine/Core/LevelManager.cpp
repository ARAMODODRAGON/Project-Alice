#include "LevelManager.hpp"
#include "Level.hpp"
#include "../Game.hpp"

LevelManager::LevelManager()
	: currentLevel(nullptr)
	, levelToLoad(nullptr)
	, levelAction(LevelAction::None) { }

LevelManager::~LevelManager() {
	// delete level
	if (currentLevel) delete currentLevel;
	currentLevel = nullptr;
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

void LevelManager::ResetActions() {
	levelAction = LevelAction::None;
	levelToLoad = nullptr;
}

void LevelManager::MakeLevel() {
	if (levelToLoad == nullptr) {
		DEBUG_ERROR("err in the level manager");
		return;
	}

	Level* level = levelToLoad();
	currentLevel = level;
	level->Init();
}
