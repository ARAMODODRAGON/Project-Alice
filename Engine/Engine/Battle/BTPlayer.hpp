#ifndef PLAYER_BTPLAYER_H
#define PLAYER_BTPLAYER_H

#include "BattleManager.hpp"
#include "../Common.hpp"

class BTPlayer : public Object {
	const float delta = 1.0f / 60.0f;
	float moveSpeed;

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
	float GetMoveSpeed();
	int GetHitpoints();
	int GetMaxHitpoints();
	Sprite* GetSprite();

	void SetMoveSpeed(float _moveSpeed);
	void SetHitpoints(int _hitpoints);
	void SetMaxHitpoints(int _maxHitpoints, bool _updateCurHP);
	void SetSprite(std::string _filepath);

	RTTR_ENABLE(Object) RTTR_REGISTRATION_FRIEND
};

#endif

