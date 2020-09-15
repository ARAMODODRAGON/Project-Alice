#include "LevelManager.hpp"
#include "Level.hpp"
#include "GameContext.hpp"

LevelManager::LevelManager(const string& levelFolder, const string& defaultLevel, FileIndex* objectIndex_, ContentHandler* content_)
	: levelIndex(nullptr)
	, objectIndex(objectIndex_)
	, content(content_)
	, currentLevel(nullptr)
	, frozenLevel(nullptr)
	, levelToLoad("")
	, levelAction(LevelAction::None) {
	if (GameContext::levelManager == nullptr)
		GameContext::levelManager = this;
	else DEBUG_ERROR("GameContext::levelManager was not null");

	// create a level index based on the given folder
	levelIndex = new FileIndex(levelFolder);

	// now load the first level
	if (levelIndex->Contains(defaultLevel)) {
		json data;
		levelIndex->GetJSON(&data, defaultLevel);
		currentLevel = new Level(this, objectIndex, content);
		currentLevel->LoadData(data);
	}
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
	objectIndex = nullptr;
	if (GameContext::levelManager == this)
		GameContext::levelManager = nullptr;
}

void LevelManager::DoLevelAction() {
	if (!currentLevel) {
		DEBUG_ERROR("No level currently loaded and so no actions can be taken");
	}

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
			json data;
			levelIndex->GetJSON(&data, levelToLoad);
			currentLevel = new Level(this, objectIndex, content);
			currentLevel->LoadData(data);

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
			json data;
			levelIndex->GetJSON(&data, levelToLoad);
			currentLevel = new Level(this, objectIndex, content);
			currentLevel->LoadData(data);

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

			// load new level
			json data;
			levelIndex->GetJSON(&data, levelToLoad);
			frozenLevel = new Level(this, objectIndex, content);
			frozenLevel->LoadData(data);

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


void LevelManager::LoadLevel(const string& level) {
	levelAction = LevelAction::Replace;
	levelToLoad = level;
}

void LevelManager::SwapAndLoadLevel(const string& level) {
	levelAction = LevelAction::SwapThenReplace;
	levelToLoad = level;
}

void LevelManager::LoadFrozen(const string& level) {
	levelAction = LevelAction::ReplaceFrozen;
	levelToLoad = level;
}

void LevelManager::UnloadFrozen() {
	levelAction = LevelAction::DeleteFrozen;
	levelToLoad = "";
}

void LevelManager::Swap() {
	levelAction = LevelAction::Swap;
	levelToLoad = "";
}

void LevelManager::UnloadAndSwap() {
	levelAction = LevelAction::UnloadAndSwap;
	levelToLoad = "";
}

void LevelManager::ResetActions() {
	levelAction = LevelAction::None;
	levelToLoad = "";
}
