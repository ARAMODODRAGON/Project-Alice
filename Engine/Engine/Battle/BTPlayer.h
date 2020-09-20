#ifndef PLAYER_BTPLAYER_H
#define PLAYER_BTPLAYER_H

#include "../Objects/Object.hpp"
#include "BattleManager.hpp"

class BTPlayer : public Object {

	int hitpoints;
	int maxHitpoints;

public:
	BTPlayer();
	virtual ~BTPlayer() = 0;
};

#endif

