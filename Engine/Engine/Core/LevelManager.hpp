#ifndef _CORE_LEVEL_MANAGER_HPP
#define _CORE_LEVEL_MANAGER_HPP
#include "../General/FileIndex.hpp"
#include "../Rendering/ContentHandler.hpp"
#include "../General/Macros.hpp"

class Level;

class LevelManager {
	PRIVATE_SINGLETON(LevelManager);

	enum class LevelAction : unsigned char {
		None,				// do nothing
		Replace				// replaces the current level with a new level
	};

	// levels
	Level* currentLevel;
	
	// loading levels
	string levelToLoad;
	LevelAction levelAction;
	FileIndex* index;

	LevelManager();
	~LevelManager();

public:

	// events
	static void Init(const string& levelFolder, const string& defaultLevel);
	static void Update();
	static void LateUpdate();
	static void Cleanup();
	static void Exit();

	static Level* GetLevel() { return Get()->currentLevel; }
	
	// load a level
	static void LoadLevel(const string& level);
	
private:

	// helper functions
	void DoLevelAction();
	void ResetActions();
	void MakeLevel();

};

#endif // !_CORE_LEVEL_MANAGER_HPP