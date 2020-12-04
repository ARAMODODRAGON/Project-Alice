#ifndef BATTLE_BATTLE_CAMERA_HPP
#define BATTLE_BATTLE_CAMERA_HPP
#include <Engine/Common.hpp>


class BattleCamera : public Object {

	Camera* cam;

public:

	BattleCamera();
	~BattleCamera();

	void Start() override;
	void LateUpdate() override;

	// getters
	Camera* GetCamera() const { return cam; }

};

#endif // !BATTLE_BATTLE_CAMERA_HPP