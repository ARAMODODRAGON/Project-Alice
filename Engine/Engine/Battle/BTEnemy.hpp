#ifndef BT_ENEMY_HPP
#define BT_ENEMY_HPP

#include "../../Engine/Objects/Object.hpp"
#include "../../Engine/Battle/BattleManager.hpp"
#include <iostream>
#include "BattleAlogrithm.hpp"

class Game;

class BTEnemy : public Object
{
	bool isMoving;
	glm::vec2 destination;
	float maxSpeed;
	float maxAcceleration;
	
public:
	
	BTEnemy();
	virtual ~BTEnemy();

	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void OnDestroy() override;
	 
	void SeekAndArrive(glm::vec2 destination_, bool isMoving_);

	inline float GetMaxSpeed() { return maxSpeed; }
	inline float GetMaxAcceleration() { return maxAcceleration; }
	inline bool GetIsMoving() { return isMoving; }
	inline glm::vec2 GetDestination() { return destination; }
	
	inline void SetMaxAcceleration(float maxAcceleration_) { maxAcceleration = maxAcceleration_; }
	inline void SetIsMoving(bool isMoving_) { isMoving = isMoving_; }
	inline void SetMaxSpeed(float movementSpeed_) { maxSpeed = movementSpeed_; }
	inline void SetDestination(glm::vec2 destination_) { destination = destination_; }

protected:


	RTTR_ENABLE(Object) RTTR_REGISTRATION_FRIEND
};
#endif

