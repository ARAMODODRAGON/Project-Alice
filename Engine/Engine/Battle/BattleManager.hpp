#ifndef _BATTLE_BATTLEMANAGER_HPP
#define _BATTLE_BATTLEMANAGER_HPP
#include "../General/Macros.hpp"
#include "../General/Types.hpp"
#include "../Core/Debugger.hpp"

// forward declare
class BTPlayer;
class BTEnemy;

class BattleManager {
	PRIVATE_SINGLETON(BattleManager);

	vector<BTPlayer*> players;
	vector<BTEnemy*> enemies;

public:

	BattleManager();
	~BattleManager();

	// adders & removers
	static void AddPlayer(BTPlayer* player);
	static void RemovePlayer(BTPlayer* player);
	static void AddEnemy(BTEnemy* enemy);
	static void RemoveEnemy(BTEnemy* enemy);

	// getters
	static size_t GetPlayerCount();
	static BTPlayer* GetPlayer(size_t index = 0);
	static size_t GetEnemyCount();
	static BTEnemy* GetEnemy(size_t index = 0);


};

#endif // !_BATTLE_BATTLEMANAGER_HPP