#ifndef _BATTLE_BATTLEMANAGER_HPP
#define _BATTLE_BATTLEMANAGER_HPP
#include "../General/Macros.hpp"
#include "../General/Types.hpp"
#include "../Core/Debugger.hpp"
#include "../Rendering/Rect.hpp"
#include "../General/FileIndex.hpp"

// forward declare
class BTPlayer;
class BTEnemy;

class BattleManager {
	PRIVATE_SINGLETON(BattleManager);

	vector<BTPlayer*> players;
	vector<BTEnemy*> enemies;
	Rect battleArea;
	FileIndex* phaseIndex;

	float timer;

	BattleManager();
	~BattleManager();

public:

	// events
	static void Init(const string& phaseFilePath);
	static void Update(const float timescale);

	// loading phases
	static json LoadPhaseFile(const string& phaseFile);

	// adders & removers
	static void AddPlayer(BTPlayer* player);
	static void RemovePlayer(BTPlayer* player);
	static void AddEnemy(BTEnemy* enemy);
	static void RemoveEnemy(BTEnemy* enemy);

	// getters & setters
	static size_t GetPlayerCount();
	static BTPlayer* GetPlayer(size_t index = 0);
	static size_t GetEnemyCount();
	static BTEnemy* GetEnemy(size_t index = 0);
	static Rect GetBattleArea();
	static void SetBattleArea(const Rect& battleArea_);
	static float GetTimer() { return Get()->timer; }
	static void SetTimer(const float timer_) { Get()->timer = timer_; }

};

#endif // !_BATTLE_BATTLEMANAGER_HPP