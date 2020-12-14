#ifndef ENEMY_CW_PHASE_2
#define ENEMY_CW_PHASE_2
#include <Engine\Battle\BTPhase.hpp>
#include <Engine\Battle\ShootComponent.hpp>

class EnemyCWPhase2 : public BTPhase {
public:

	EnemyCWPhase2();
	~EnemyCWPhase2();

private:

	virtual void Init() override;
	virtual void StartPhase() override;
	virtual void UpdatePhase() override;
	virtual bool IsComplete() override;
	virtual void PlayerDirection(glm::vec2 enemyPos_, uint8 dir_) override;

};

#endif // !ENEMY_CW_PHASE_2