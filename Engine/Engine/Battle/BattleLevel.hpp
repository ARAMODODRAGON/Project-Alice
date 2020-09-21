#ifndef _BATTLE_BATTLE_LEVEL_HPP
#define _BATTLE_BATTLE_LEVEL_HPP
#include "../Core/Level.hpp"

class BattleLevel : public ILevel {

public:

	// inherited events
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Exit() override;
	virtual void Init();

};

#endif // !_BATTLE_BATTLE_LEVEL_HPP