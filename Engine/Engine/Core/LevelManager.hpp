#ifndef _CORE_LEVEL_MANAGER_HPP
#define _CORE_LEVEL_MANAGER_HPP
#include "../General/FileIndex.hpp"
#include "../Rendering/ContentHandler.hpp"
#include "../General/Macros.hpp"
#include "../Battle/TileMap/TileMap.hpp"
#include "../Battle/TileMap/Tile.hpp"

class Level;

class LevelManager {
	PRIVATE_SINGLETON(LevelManager);
	using LevelBinding = Level * (*)();

	enum class LevelAction : unsigned char {
		None,				// do nothing
		Replace				// replaces the current level with a new level
	};

	// levels
	Level* currentLevel;

	// loading levels
	LevelBinding levelToLoad;
	LevelAction levelAction;


	LevelManager();
	~LevelManager();

public:

	// events
	template<typename LevelToLoad>
	static void Init();
	static void Update();
	static void LateUpdate();
	static void Cleanup();
	static void Exit();

	static Level* GetLevel() { return Get()->currentLevel; }

	// load a level
	template<typename LevelToLoad>
	static void LoadLevel();

private:

	// helper functions
	void DoLevelAction();
	void ResetActions();
	void MakeLevel();

};

template<typename LevelToLoad>
inline void LevelManager::Init() {
	// load the first level
	LoadLevel<LevelToLoad>();
	Get()->DoLevelAction();
}

template<typename LevelToLoad>
inline void LevelManager::LoadLevel() {
	Get()->levelAction = LevelAction::Replace;
	Get()->levelToLoad = []()->Level* { return new LevelToLoad(); };
}

#endif // !_CORE_LEVEL_MANAGER_HPP