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

	inline float GetMaxHealth() { return maxHealth; }
	inline float SetMaxHealth(const float maxHealth_) { maxHealth = maxHealth_; }
	inline float GetCurrentHealth() { return currentHealth; }
	inline float SetCurrentHealth(const float currentHealth_) { currentHealth = currentHealth_; }

	RTTR_ENABLE(Object) RTTR_REGISTRATION_FRIEND
};
#endif

