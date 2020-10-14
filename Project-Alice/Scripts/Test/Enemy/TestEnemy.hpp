#ifndef TEST_ENEMY_HPP
#define TEST_ENEMY_HPP

#include "Engine/Battle/BTEnemy.hpp"
#include "../Phases/TestPhase.hpp"
#include "Engine/Common.hpp"
#include <iostream>
#include <cstdlib>
#include <time.h>




class TestEnemy : public BTEnemy
{
	//vec2 destination;
public:

	SpriteRenderer* sprite;
	TestPhase* moveFromMiddle;

	TestEnemy();
	virtual ~TestEnemy();


	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void OnDestroy() override;


	RTTR_ENABLE(BTEnemy) RTTR_REGISTRATION_FRIEND
};
#endif // !TEST_ENEMY_HPP

