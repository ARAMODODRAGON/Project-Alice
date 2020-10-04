#ifndef BT_PHASE_HPP
#define BT_PHASE_HPP

#include "../General/Serialization.hpp"
#include <vector>
#include <string>
#include <glm/glm.hpp>


class BTEnemy;
class BattleManager;



class BTPhase
{
	std::string nextPhase;
	std::string phaseName;

	int startingHealth;		//enemy starting health
	float startingTime;

	BTEnemy* enemy;
	
public:

	BTPhase();
	virtual ~BTPhase() = 0;

	virtual void UpdatePhase() = 0;		//updates the phase logic 
	virtual bool isComplete() = 0;		//checks if the phase is complete 

	inline void MoveTo(glm::vec2 destination_);

#pragma region Getters
	inline std::string GetNextPhase() { return nextPhase; }
	inline std::string GetPhaseName() { return phaseName; }
	inline int GetStartingHealth() { return startingHealth; }
	inline float GetstartingTime() { return startingTime; }
	inline BTEnemy* GetEnemy() { return enemy; }
#pragma endregion

	//the poor left behind setter
	inline void SetEnemy(BTEnemy* enemy_) { enemy = enemy_; }

protected:

#pragma region Setters
	inline void SetNextPhase(std::string nextPhase_) { nextPhase = nextPhase_; }
	inline void SetStartingHealth(int startingHealth_) { startingHealth = startingHealth_; }
	inline void SetStartingTime(float startingTime_) { startingTime = startingTime_; }
	inline void SetPhaseName(std::string phaseName_) { phaseName = phaseName_; }
#pragma endregion

	RTTR_ENABLE() RTTR_REGISTRATION_FRIEND

};
#endif

