#ifndef BATTLE_BATTLE_DEMO_CAMERA_HPP
#define BATTLE_BATTLE_DEMO_CAMERA_HPP
#include <Engine\Objects\Object.hpp>
#include <Engine\Rendering\Camera.hpp>
#include <Engine\Rendering\UIRenderer.hpp>

class BattleDemoCamera : public Object {

	Camera* camera;
	UIRenderer* ui;

public:

	BattleDemoCamera();
	~BattleDemoCamera();

	virtual void Start() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void OnDestroy() override;

	// getters
	Camera* GetCamera() const { return camera; }
	UIRenderer* GetUI() const { return ui; }

	RTTR_ENABLE() RTTR_REGISTRATION_FRIEND
};

#endif // !BATTLE_BATTLE_DEMO_CAMERA_HPP