#ifndef ENEMY_CW_PHASE_0
#define ENEMY_CW_PHASE_0
#include <Engine\Battle\BTPhase.hpp>
#include <Engine\Battle\ShootComponent.hpp>

class EnemyCWPhase0 : public BTPhase {
public:

	enum State : uint8 {
		State_Shoot4,
		State_Shoot3,
		State_Move,
		State_Center,
		State_ShootCircleAndTarget
	};

	EnemyCWPhase0();
	~EnemyCWPhase0();

private:

	uint8 state;
	uint8 repeatState;
	ShootComponent* shc;

	virtual void Init() override;
	virtual void StartPhase() override;
	virtual void UpdatePhase() override;
	virtual bool IsComplete() override;

	RTTR_ENABLE(BTPhase) RTTR_REGISTRATION_FRIEND
};

#endif // !ENEMY_CW_PHASE_0