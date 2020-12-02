#ifndef BT_PHASE_HPP
#define BT_PHASE_HPP

#include "../General/Serialization.hpp"
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "BTEnemy.hpp"
#include <chrono>



class BTEnemy;
class BattleManager;



class BTPhase
{
	std::string nextPhase;
	std::string phaseName;

	int currentHealth;		//enemy starting health
	int maxHealth;

	float startingTime;
	float acceleration;
	float maxSpeed;

	glm::vec2 destination;
	glm::vec2 position;

	BTEnemy* enemy;
	
	

public:
	BTPhase();
	virtual ~BTPhase() = 0;

	virtual void Init() {}
	virtual void StartPhase() = 0;       // init all starting values 
	virtual void UpdatePhase() = 0;		//updates the phase logic 
	virtual bool IsComplete() = 0;		//checks if the phase is complete 


	//getters and setters for enemy values as the phases will control the ai side of the enemys 
#pragma region Getters
	inline std::string GetNextPhase() { return nextPhase; }
	inline std::string GetPhaseName() { return phaseName; }

	inline glm::vec2 GetPhaseDestination() { return destination; }
	inline glm::vec2 GetEnemyPosition() { return position; }

	inline int GetEnemyCurrentHealth() { return currentHealth; }
	inline int GetEnemyMaxHealth() { return maxHealth; }

	inline float GetstartingTime() { return startingTime; }
	inline float GetEnemyAcceleration() { return acceleration; }
	inline float GetEnemyMaxSpeed() { return maxSpeed; }

	inline BTEnemy* GetEnemy() { return enemy; }

#pragma endregion

	//the poor left behind setter
	inline void SetEnemy(BTEnemy* enemy_) { enemy = enemy_; }

protected:

#pragma region Setters
	inline void SetAcceleration(float accereration_) { acceleration = accereration_; }
	inline void SetMaxSpeed(float maxSpeed_) { maxSpeed = maxSpeed_; }
	inline void SetStartingTime(float startingTime_) { startingTime = startingTime_; }

	inline void SetCurrentHealth(int startingHealth_) { currentHealth = startingHealth_; }
	inline void SetMaxHealth(int maxHealth_) { maxHealth = maxHealth_; }

	inline void SetNextPhase(std::string nextPhase_) { nextPhase = nextPhase_; }
	inline void SetPhaseName(std::string phaseName_) { phaseName = phaseName_; }

	inline void SetPosition(glm::vec2 position_) { position = position_; }
	inline void SetPhaseDestination(glm::vec2 destination_) { destination = destination_; }
#pragma endregion

	RTTR_ENABLE() RTTR_REGISTRATION_FRIEND

};
#endif



