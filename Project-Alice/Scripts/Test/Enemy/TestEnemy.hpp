#ifndef TEST_ENEMY_HPP
#define TEST_ENEMY_HPP
#include "Engine/Battle/BTEnemy.hpp"
#include "Engine/Common.hpp"
#include <iostream>
#include <cstdlib>
#include <time.h>



class TestEnemy : public BTEnemy
{
	//vec2 destination;
public:

	Sprite* sprite;

	TestEnemy();
	~TestEnemy();


	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void OnDestroy() override;

	//TODO:
	//needs to be moved to BT ENENMY
	//void SeekAndArrive(bool isMoving_);
	//vec2 GetDestination() { return destination; }
	//void SetDestination(vec2 destination_) { destination = destination_; }

	RTTR_ENABLE(BTEnemy) RTTR_REGISTRATION_FRIEND
};
#endif // !TEST_ENEMY_HPP

