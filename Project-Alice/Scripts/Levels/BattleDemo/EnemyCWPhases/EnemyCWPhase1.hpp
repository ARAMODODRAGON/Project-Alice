#ifndef ENEMY_CW_PHASE_1
#define ENEMY_CW_PHASE_1
#include <Engine\Battle\BTPhase.hpp>
#include <Engine\Battle\ShootComponent.hpp>

class EnemyCWPhase1 : public BTPhase {
public:

	EnemyCWPhase1();
	~EnemyCWPhase1();

private:

	virtual void Init() override;
	virtual void StartPhase() override;
	virtual void UpdatePhase() override;
	virtual bool IsComplete() override;

};

#endif // !ENEMY_CW_PHASE_1