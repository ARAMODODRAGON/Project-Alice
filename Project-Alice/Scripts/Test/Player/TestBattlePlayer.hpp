#ifndef TEST_BTPLAYER_H
#define TEST_BTPLAYER_H

#include <Engine/Battle/BTPlayer.hpp>
#include "../../Overworld/SpellInventory.hpp"

class TestBattlePlayer : public BTPlayer {

public:
	TestBattlePlayer();
	~TestBattlePlayer();

	void Start() override;

	RTTR_ENABLE(Object) RTTR_REGISTRATION_FRIEND
};

#endif