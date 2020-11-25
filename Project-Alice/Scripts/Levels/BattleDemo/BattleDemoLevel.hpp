#ifndef BATTLE_BATTLE_DEMO_LEVEL_HPP
#define BATTLE_BATTLE_DEMO_LEVEL_HPP
#include <Engine\Core\Level.hpp>
#include "BattleDemoCamera.hpp"

class BattleDemoLevel : public Level {
public:

	BattleDemoLevel();
	~BattleDemoLevel();

	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Exit() override;

private:

	BattleDemoCamera* camera;

	RTTR_ENABLE() RTTR_REGISTRATION_FRIEND
};

#endif // !BATTLE_BATTLE_DEMO_LEVEL_HPP