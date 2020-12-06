#include "BTEnemy.hpp"
#include "../Game.hpp"
#include "BTPhase.hpp"

BTEnemy::BTEnemy() : currentHealth(0.0f), currentPhase(-1), maxHealth(0.0f) {
	BattleManager::AddEnemy(this);

}

BTEnemy::~BTEnemy() {
	OnDestroy();
	BattleManager::RemoveEnemy(this);
}

void BTEnemy::Start() {

}

void BTEnemy::Update() {

	if (phases.empty()) {
		DEBUG_ERROR("There are no phases in the phases Vector");
	}

	//right now only goes through the phases 0,1,2 etc....
	//when current phase is greater than the size go back to the begining of the vector 

	else {

		if (currentPhase == -1) {
			currentPhase = 0;
			phases[currentPhase]->StartPhase();
		}

		if (!phases[currentPhase]->IsComplete()) {

			phases[currentPhase]->UpdatePhase();
		}

		if (phases[currentPhase]->IsComplete()) {
			currentPhase += 1;

			if (currentPhase > phases.size() - 1) {
				currentPhase = 0;
				phases[currentPhase]->StartPhase();
				return;
			}

			phases[currentPhase]->StartPhase();
		}
	}
}

void BTEnemy::LateUpdate() { }

void BTEnemy::OnDestroy() {
	for (auto phase : phases) {
		phase = nullptr;
	}
	phases.clear();

}

BTPhase* BTEnemy::GetPhase(const string& phaseName_) {
	for (BTPhase* phase : phases) {

		if (phase->GetPhaseName() == phaseName_) {
			return phase;
		}
	}

	DEBUG_ERROR("No phase found with name " + phaseName_);
	return nullptr;
}







