#ifndef _BATTLE_BATTLE_LEVEL_HPP
#define _BATTLE_BATTLE_LEVEL_HPP
#include "../Core/Level.hpp"

class BattleLevel : public Level {

public:

	// inherited events
	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Exit() override;

	RTTR_ENABLE(Level) RTTR_REGISTRATION_FRIEND
};

#endif // !_BATTLE_BATTLE_LEVEL_HPP