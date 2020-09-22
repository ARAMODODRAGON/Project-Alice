#ifndef PLAYER_BTPLAYER_H
#define PLAYER_BTPLAYER_H

#include "BattleManager.hpp"
#include "../Common.hpp"

class BTPlayer : public Object {

	int hitpoints;
	int maxHitpoints;

	Sprite* sprite;

public:
	BTPlayer();
	virtual ~BTPlayer() = 0;

	void Start() override;
	void Update() override;
	void LateUpdate() override;

	void UpdateHitpoints(int _modifier);

	// Getters and Setters
	int GetHitpoints() { return hitpoints; }
	int GetMaxHitpoints() { return maxHitpoints; }

	void SetHitpoints(int _hitpoints) { hitpoints = _hitpoints; }
	void SetMaxHitpoints(int _maxHitpoints) { maxHitpoints = _maxHitpoints; }

	RTTR_ENABLE(Object) RTTR_REGISTRATION_FRIEND
};

#endif

