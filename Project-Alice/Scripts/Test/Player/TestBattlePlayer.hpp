#ifndef TEST_BTPLAYER_H
#define TEST_BTPLAYER_H

#include <Engine/Battle/BTPlayer.hpp>
#include <Engine/Content/AudioSourceComponent.hpp>
#include "../../Overworld/SpellInventory.hpp"

class TestBattlePlayer : public BTPlayer {
	AudioSourceComponent* audioSource;
public:
	TestBattlePlayer();
	~TestBattlePlayer();

	void Start() override;

	RTTR_ENABLE(Object) RTTR_REGISTRATION_FRIEND
};

#endif