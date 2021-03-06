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

	template<class T> T* AddPhase() {
		// derived
		if (std::is_base_of<BTPhase, T>::value) {
			// reinterpret is allowed because we know its dirvetive
			T* phaseT = new T();
			BTPhase* phase = reinterpret_cast<BTPhase*>(phaseT);
			phase->SetEnemy(this);
			phases.push_back(phase); 
			phase->Init();
			return phaseT;
		}
		// not derived
		DEBUG_ERROR("Given type does not inherit from BTPhase");
		return nullptr;
	}
	BTPhase* GetPhase(const string& phaseName_); // return a phase by the name given

	inline float GetMaxHealth() { return maxHealth; }
	inline void SetMaxHealth(const float maxHealth_) { maxHealth = maxHealth_; }
	inline float GetCurrentHealth() { return currentHealth; }
	inline void SetCurrentHealth(const float currentHealth_) { currentHealth = currentHealth_; }

};
#endif

