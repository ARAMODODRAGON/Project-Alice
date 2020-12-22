#ifndef ALICE_CHARACTERS_DEMOCHARA_HPP
#define ALICE_CHARACTERS_DEMOCHARA_HPP
#include <ALC\Entities\EntityCreationHandler.hpp>
#include <ALC\Bullets\BulletPhysics.hpp>
#include <ALC\Content\ContentManager.hpp>
#include <ALC\Rendering\SpriteBatch.hpp>
#include <ALC\Input\Keyboard.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <ALC\StateMachine.hpp>

struct DemoBulletComponent final {
	float lifetime = 0.0f;
	float maxlifetime = 3.0f;
};

class DemoChara final : public ALC::Behavior {
public:

	DemoChara();
	~DemoChara();

	void Start(ALC::Entity self) override;
	void OnDestroy(ALC::Entity self) override;

	void Update(ALC::Entity self, ALC::Timestep ts) override;
	void LateUpdate(ALC::Entity self, ALC::Timestep ts) override;

private:

	ALC::StateMachine<DemoChara, ALC::uint32, ALC::Entity, ALC::Timestep> m_state;
	float m_timer;
	float m_circleshootoffset;
	bool m_clockwise;
	float m_spinspeedmult;
};


#endif // !ALICE_CHARACTERS_DEMOCHARA_HPP