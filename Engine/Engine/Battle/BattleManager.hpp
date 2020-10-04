#ifndef _BATTLE_BATTLEMANAGER_HPP
#define _BATTLE_BATTLEMANAGER_HPP
#include "../General/Macros.hpp"
#include "../General/Types.hpp"
#include "../Core/Debugger.hpp"
#include "../Rendering/Rect.hpp"


// forward declare
class BTPlayer;
class BTEnemy;

class BattleManager {
	PRIVATE_SINGLETON(BattleManager);

	vector<BTPlayer*> players;
	vector<BTEnemy*> enemies;
	Rect battleArea;

	BattleManager();
	~BattleManager();

public:

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

};

#endif // !_BATTLE_BATTLEMANAGER_HPP