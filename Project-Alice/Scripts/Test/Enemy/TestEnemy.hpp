#ifndef TEST_ENEMY_HPP
#define TEST_ENEMY_HPP

#include "Engine/Battle/BTEnemy.hpp"
#include "../Phases/TestPhase.hpp"
#include "../Phases/TestPhase1.hpp"
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
	TestPhase1* moveToMiddle;

	TestEnemy();
	virtual ~TestEnemy();


	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void OnDestroy() override;


};
#endif // !TEST_ENEMY_HPP

