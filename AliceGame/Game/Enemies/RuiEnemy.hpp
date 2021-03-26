
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
		Moving,
		TwinSpin,
		RainBullets,
		CircleBombs,
		ZigZagShot
	};

	// contains the current phase
	ALC::EntityStateMachine<RuiEnemy, Phase> m_phases;
	// state that depends on the current phase
	State m_state;
	State m_prevState;
	ALC::Texture m_bulletTexture;
	ALC::vector<MoveStates::States> moveStates; //  a vector used to hold the move states for the phase 

	bool canMove = true;
	bool canShoot;
	//all below timers are used for fire rate purposes 
	float m_timer; 
	float m_secondTimer; // yet another timer 
	float m_thirdTimer; // gosh i love my timers 

	 //except for these two 
	float plyrMoveTimer; //another Timer? jeez // anyways used to check how long the player has been in one position 
	float stateTimer;  //  not really needed but used for one phase to call an end to the shooting phase 

	ALC::uint8 dirIndex = 0;// an index to the direction array
	ALC::uint8 rainPosIndex = 0; // used to determin which pos to drop bullets from the top 
	ALC::uint8 shotCounter = 0;	// used for pahse 3 so the AI dosent shoot all the time 

	// used to check if the player has moved 
	ALC::vec2 oldPos;
	ALC::vec2 plyrOldPos;

	ALC::vec2 dir[4] = {
		ALC::vec2(0,1),	  //up
		ALC::vec2(1,0),	  //right
		ALC::vec2(0,-1),  //down
		ALC::vec2(-1,0)	 //left
	};

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

	//move To Enemy?
	//void ChangeDirectionState(State curntState);
	//void ChangeState(State nextState);

	void ResteTimer() { m_timer = m_secondTimer = 0.0f; }

};

#endif // !ALICE_ENEMIES_RUIENEMY_HPP