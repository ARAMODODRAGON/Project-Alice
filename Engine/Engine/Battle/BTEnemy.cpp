#include "BTEnemy.hpp"
#include "../Game.hpp"

RTTR_REGISTRATION{
	registration::class_<BTEnemy>("BTEnemy")
		.property("movmentSpeed",&BTEnemy::maxSpeed)
		.property("destination",&BTEnemy::destination)
		.property("isMoving",&BTEnemy::isMoving)
		.property("maxAcceleration",&BTEnemy::maxAcceleration);
}

BTEnemy::BTEnemy() : maxSpeed(NULL),destination(vec2()),isMoving(false) { BattleManager::AddEnemy(this); }

BTEnemy::~BTEnemy() { BattleManager::RemoveEnemy(this); }

void BTEnemy::Start()
{
}

void BTEnemy::Update()
{
}

void BTEnemy::LateUpdate()
{
}

void BTEnemy::OnDestroy()
{
	

}

void BTEnemy::SeekAndArrive(glm::vec2 destination_, bool isMoving_)
{
	
	if (!isMoving_) {

		SetVelocity(bta::MoveTo(GetPosition(), destination_, 10 ,maxSpeed));
		isMoving = true;
	}
	
	else if(isMoving_) {
		glm::vec2 tempPosition = glm::round(GetPosition());

		if (NearlyEqual(GetPosition(), destination_,3.0f)) {
			isMoving = false;
		}
	}



}



