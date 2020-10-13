#ifndef BT_ENEMY_HPP
#define BT_ENEMY_HPP

#include "../../Engine/Objects/Object.hpp"
#include "../../Engine/Battle/BattleManager.hpp"
#include <iostream>
#include "BattleAlogrithm.hpp"

class Game;
class BTPhase;

class BTEnemy : public Object
{
	glm::vec2 destination;
	float maxSpeed;
	float maxAcceleration;
	float maxHealth;
	float currentHealth;
	std::vector<BTPhase*> phases;
	int currentPhase;

public:
	BTEnemy();
	virtual ~BTEnemy();

	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void OnDestroy() override;

	BTPhase* AddPhase(const type type_); //add a phase 
	BTPhase* GetPhase(const string& phaseName_); // return a phase by the name given

	inline float GetMaxSpeed() { return maxSpeed; }
	inline float GetMaxAcceleration() { return maxAcceleration; }
	inline float GetMaxHealth() { return maxHealth; }
	inline float GetCurrentHealth() { return currentHealth; }
	//inline bool GetIsMoving() { return isMoving; }
	inline glm::vec2 GetDestination() { return destination; }

	inline void SetMaxAcceleration(float maxAcceleration_) { maxAcceleration = maxAcceleration_; }
	//inline void SetIsMoving(bool isMoving_) { isMoving = isMoving_; }
	inline void SetMaxSpeed(float movementSpeed_) { maxSpeed = movementSpeed_; }
	inline void SetDestination(glm::vec2 destination_) { destination = destination_; }



	RTTR_ENABLE(Object) RTTR_REGISTRATION_FRIEND
};
#endif

