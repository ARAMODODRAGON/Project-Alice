#ifndef _CORE_LEVEL_MANAGER_HPP
#define _CORE_LEVEL_MANAGER_HPP
#include "../Objects/FileIndex.hpp"
#include "../Rendering/ContentHandler.hpp"
#include "../General/Macros.hpp"
#include <functional>

class ILevel;

class LevelManager {
	PRIVATE_SINGLETON(LevelManager);

	enum class LevelAction : unsigned char {
		None,				// do nothing
		Replace				// replaces the current level with a new level
	};

	// levels
	ILevel* currentLevel;
	
	// loading levels
	string levelToLoad;
	LevelAction levelAction;

	LevelManager();
	~LevelManager();

public:

	// events
	static void Init(const string& defaultLevel);
	static void Update();
	static void LateUpdate();
	static void Clean();
	static void Exit();

	static ILevel* GetLevel() { return Get()->currentLevel; }
	
	// load a level
	static void LoadLevel(const string& level);
	
private:

	// helper functions
	void DoLevelAction();
	void ResetActions();
	void MakeLevel();

};

#endif // !_CORE_LEVEL_MANAGER_HPP