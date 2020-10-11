#include "BTEnemy.hpp"
#include "../Game.hpp"
#include "BTPhase.hpp"

RTTR_REGISTRATION{
	registration::class_<BTEnemy>("BTEnemy")
		.public_object_constructor
		.property("movmentSpeed",&BTEnemy::maxSpeed)
		.property("destination",&BTEnemy::destination)
		.property("maxAcceleration",&BTEnemy::maxAcceleration)
		.property("phases",&BTEnemy::phases)
		.property("currentPhase",&BTEnemy::currentPhase);
}

BTEnemy::BTEnemy() : maxSpeed(NULL),destination(vec2()),maxAcceleration(NULL) { 
	BattleManager::AddEnemy(this);
	

}

BTEnemy::~BTEnemy() { BattleManager::RemoveEnemy(this); }

void BTEnemy::Start()
{
	
}

void BTEnemy::Update()
{
	if (phases[currentPhase]) {
		if (!phases[currentPhase]->isComplete()) {
			phases[currentPhase]->StartPhase();
			phases[currentPhase]->UpdatePhase();
		}

		if (phases[currentPhase]->isComplete() && currentPhase < phases.size() - 1) {
			currentPhase++;
		}
		//else if(phases[currentPhase])
		else if (phases[currentPhase]) {

		}
	}
}

void BTEnemy::LateUpdate()
{
}

void BTEnemy::OnDestroy()
{
	

}

BTPhase* BTEnemy::AddPhase(const type type_)
{
	if (!type_.is_derived_from(type::get<BTPhase>())) {
		DEBUG_ERROR("Type : " + type_.get_name() + "is not of type BTPhase");
		return nullptr;
	}

	variant phaseObj = type_.get_constructor().invoke();
	if (!phaseObj) {
		DEBUG_ERROR("Could not construct phase of type : " + type_.get_name());
		return nullptr;
	}

	BTPhase* phase_ = phaseObj.get_value<BTPhase*>();
	if (!phase_) {
		DEBUG_ERROR("could not instatiate phase : " + type_.get_name());
		return nullptr;
	}

	phases.push_back(phase_);
	return phase_;

}

BTPhase* BTEnemy::GetPhase(const string& phaseName_)
{
	for (BTPhase* phase : phases) {
		
		if (phase->GetPhaseName() == phaseName_) {
			return phase;
		}
	}

	DEBUG_ERROR("No phase found with name " + phaseName_);
	return nullptr;
}






