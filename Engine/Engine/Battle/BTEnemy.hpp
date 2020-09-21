#ifndef BT_ENEMY_HPP
#define BT_ENEMY_HPP

#include "../../Engine/Objects/Object.hpp"
#include "../../Engine/Battle/BattleManager.hpp"
#include <iostream>

using namespace std;
class BTEnemy : public Object
{
public:
	float movmentSpeed;

	BTEnemy();
	virtual ~BTEnemy();

	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void OnDestroy() override;


	RTTR_ENABLE(Object) RTTR_REGISTRATION_FRIEND
};
#endif

