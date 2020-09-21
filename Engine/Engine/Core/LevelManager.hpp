#ifndef _CORE_LEVEL_MANAGER_HPP
#define _CORE_LEVEL_MANAGER_HPP
#include "../Objects/FileIndex.hpp"
#include "../Rendering/ContentHandler.hpp"
#include "../General/Macros.hpp"

class Level;

class LevelManager {
	PRIVATE_SINGLETON(LevelManager);

	enum class LevelAction : unsigned char {
		None,				// do nothing
		Replace				// replaces the current level with a new level
		//SwapThenReplace,	// swaps the current level with the frozen one, then replaces the now current level
		//ReplaceFrozen,	// loads a level into the frozen spot, replaces any level already loaded
		//DeleteFrozen,		// deletes the level in the frozen slot
		//Swap,				// swaps the current level with the frozen one
		//UnloadAndSwap		// unloads the current level and then swaps to the frozen one
	};

	// indexes used for searching
	FileIndex* levelIndex; // required for level loading
	FileIndex* objectIndex; // passes down to every level

	// levels
	Level* currentLevel;
	//Level* frozenLevel;

	// loading levels
	string levelToLoad;
	LevelAction levelAction;

	LevelManager();
	~LevelManager();

public:

	// events
	static void Init(const string& levelFolder, const string& defaultLevel, FileIndex* objectIndex_ = nullptr);
	static void Update();
	static void LateUpdate();
	static void Draw();
	static void Clean();
	static void Exit();

	static Level* GetLevel() { return Get()->currentLevel; }
	
	/// level action functions (Note: any action will override eachother)

	// replaces the current level with a new one
	static void LoadLevel(const string& level);
	// replaces the frozen level and then swaps to it
	//static void SwapAndLoadLevel(const string& level);
	// replaces the frozen level
	//static void LoadFrozen(const string& level);
	// deletes the frozen level (if there is one)
	//static void UnloadFrozen();
	// swaps the frozen and unfrozen levels
	//static void Swap();
	// unloads the current level and swaps to the frozen one
	//static void UnloadAndSwap();

private:

	// helper functions
	void DoLevelAction();
	void ResetActions();
	void MakeLevel(const string& levelToLoad_);

};

#endif // !_CORE_LEVEL_MANAGER_HPP