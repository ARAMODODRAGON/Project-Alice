#ifndef TEST_BTPLAYER_H
#define TEST_BTPLAYER_H

#include "BTPlayer.h"

class TestBattlePlayer : public BTPlayer {

public:
	TestBattlePlayer();
	~TestBattlePlayer();

	void Start() override;
	void Update() override;
	void LateUpdate() override;

	RTTR_ENABLE(Object) RTTR_REGISTRATION_FRIEND
};

#endif