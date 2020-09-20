#ifndef TEST_ENEMY_HPP
#define TEST_ENEMY_HPP
#include "Engine/Objects/BTEnemy.hpp"
#include "Engine/Common.hpp"
#include <iostream>


using namespace std;
class TestEnemy : public BTEnemy
{
public:

	Sprite* sprite;

	TestEnemy();
	~TestEnemy();

	void SeekAndArrive(vec2 destination_, vec2 enemyPosition_);
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void OnDestroy() override;

	RTTR_ENABLE(BTEnemy) RTTR_REGISTRATION_FRIEND
};
#endif // !TEST_ENEMY_HPP

