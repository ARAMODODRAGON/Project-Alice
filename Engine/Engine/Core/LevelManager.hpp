#ifndef _CORE_LEVEL_MANAGER_HPP
#define _CORE_LEVEL_MANAGER_HPP
#include "../Objects/FileIndex.hpp"

class Level;

class LevelManager {

	enum class LevelAction : unsigned char {
		None,				// do nothing
		Replace,			// replaces the current level with a new level
		SwapThenReplace,	// swaps the current level with the frozen one, then replaces the now current level
		ReplaceFrozen,		// loads a level into the frozen spot, replaces any level already loaded
		DeleteFrozen,		// deletes the level in the frozen slot
		Swap,				// swaps the current level with the frozen one
		UnloadAndSwap		// unloads the current level and then swaps to the frozen one
	};

	// indexes used for searching
	FileIndex* levelIndex; // required for level loading
	FileIndex* objectIndex; // passes down to every level

	// levels
	Level* currentLevel;
	Level* frozenLevel;
	string levelToLoad;
	LevelAction levelAction;

	// functions
	//boost::function1<Level*, const string&> creationMethod;
	//boost::function1<void, Level*> destructionMethod;

public:

	LevelManager(const string& levelFolder, const string& defaultLevel, FileIndex* objectIndex_ = nullptr);
	~LevelManager();

	Level* GetLevel() const { return currentLevel; }
	// this must be called for any of the level action functions to work
	void DoLevelAction();

	// level action functions (Note: any action will override eachother)

	// replaces the current level with a new one
	void LoadLevel(const string& level);
	// replaces the frozen level and then swaps to it
	void SwapAndLoadLevel(const string& level);
	// replaces the frozen level
	void LoadFrozen(const string& level);
	// deletes the frozen level (if there is one)
	void UnloadFrozen();
	// swaps the frozen and unfrozen levels
	void Swap();
	// unloads the current level and swaps to the frozen one
	void UnloadAndSwap();

private:

	// helper functions
	void ResetActions();

};

#endif // !_CORE_LEVEL_MANAGER_HPP