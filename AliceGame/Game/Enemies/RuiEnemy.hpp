#ifndef ALICE_ENEMIES_RUIENEMY_HPP
#define ALICE_ENEMIES_RUIENEMY_HPP
#include "Enemy.hpp"
#include <ALC\StateMachine.hpp>

class RuiEnemy final : public Enemy {
public:

	RuiEnemy();
	~RuiEnemy();

	void Start(ALC::Entity self) override;
	void Update(ALC::Entity self, ALC::Timestep ts) override;
	void LateUpdate(ALC::Entity self, ALC::Timestep ts) override;
	void OnDeath(ALC::Entity self) override;

	// called by the scene when the battle begins
	void BattleBegin() override;

private:

	enum class Phase : ALC::uint8 {
		PreBattle,
		Phase0,
		Phase1,
		Phase2,
		Phase3,
		Phase4,
		PostBattle
	};

	enum class State : ALC::uint8 {
		None,
		Shoot4,
		Shoot3,
		Move,
		Center,
		ShootCircleAndTarget
	};

	// contains the current phase
	ALC::EntityStateMachine<RuiEnemy, Phase> m_phases;
	// state that depends on the current phase
	State m_state;

	// phases

	void PreBattleBegin(const Phase lastphase, ALC::Entity self, ALC::Timestep ts);
	void PreBattleStep(ALC::Entity self, ALC::Timestep ts);
	
	void Phase0Begin(const Phase lastphase, ALC::Entity self, ALC::Timestep ts);
	void Phase0Step(ALC::Entity self, ALC::Timestep ts);
	
	void Phase1Begin(const Phase lastphase, ALC::Entity self, ALC::Timestep ts);
	void Phase1Step(ALC::Entity self, ALC::Timestep ts);
	
	void Phase2Begin(const Phase lastphase, ALC::Entity self, ALC::Timestep ts);
	void Phase2Step(ALC::Entity self, ALC::Timestep ts);
	
	void Phase3Begin(const Phase lastphase, ALC::Entity self, ALC::Timestep ts);
	void Phase3Step(ALC::Entity self, ALC::Timestep ts);
	
	void Phase4Begin(const Phase lastphase, ALC::Entity self, ALC::Timestep ts);
	void Phase4Step(ALC::Entity self, ALC::Timestep ts);
	
	void PostBattleBegin(const Phase lastphase, ALC::Entity self, ALC::Timestep ts);
	void PostBattleStep(ALC::Entity self, ALC::Timestep ts);

};

#endif // !ALICE_ENEMIES_RUIENEMY_HPP