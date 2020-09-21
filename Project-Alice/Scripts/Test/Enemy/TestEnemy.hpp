#ifndef TEST_ENEMY_HPP
#define TEST_ENEMY_HPP
#include "Engine/Battle/BTEnemy.hpp"
#include "Engine/Common.hpp"
#include <iostream>
#include <cstdlib>
#include <time.h>


using namespace std;
using namespace glm;
class TestEnemy : public BTEnemy
{
	vec2 destination;
	bool isMoving;
public:

	Sprite* sprite;

	TestEnemy();
	~TestEnemy();

	void SeekAndArrive(bool isMoving_);
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void OnDestroy() override;

	vec2 GetDestination() { return destination; }
	void SetDestination(vec2 destination_) { destination = destination_; }

	RTTR_ENABLE(BTEnemy) RTTR_REGISTRATION_FRIEND
};
#endif // !TEST_ENEMY_HPP

