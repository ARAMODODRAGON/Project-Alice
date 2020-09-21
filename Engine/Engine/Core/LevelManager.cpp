#include "LevelManager.hpp"
#include "Level.hpp"
#include "../Game.hpp"

LevelManager::LevelManager()
	: currentLevel(nullptr)
	, levelToLoad("")
	, levelAction(LevelAction::None) { }

LevelManager::~LevelManager() {
	// delete levels
	if (currentLevel) delete currentLevel;
	currentLevel = nullptr;
}

void LevelManager::Init(const string& defaultLevel) {
	// load the first level
	LoadLevel(defaultLevel);
	Get()->DoLevelAction();
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

void LevelManager::Clean() {
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
			currentLevel->Exit();
			delete currentLevel;

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
	// create level
	currentLevel = Game::Get()->LevelLoad(levelToLoad);
	DEBUG_LOG("Loaded level \"" + levelToLoad + "\" as primary");
}
