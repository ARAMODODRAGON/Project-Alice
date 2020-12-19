#ifndef ALICE_CHARACTERS_DEMOCHARA_HPP
#define ALICE_CHARACTERS_DEMOCHARA_HPP
#include <ALC\Entities\EntityCreationHandler.hpp>
#include <ALC\Physics\Rigidbody2D.hpp>
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

	ALC::array<ALC::Texture, 22> textures;

	DemoChara();
	~DemoChara();

	void Start(ALC::Entity self) override;
	void OnDestroy(ALC::Entity self) override;

	void Shoot(ALC::Entity self, const float angle, const float speed, 
			   const ALC::vec2 position, const ALC::vec4 color);
	
	void Update(ALC::Entity self, ALC::Timestep t) override;
	void LateUpdate(ALC::Entity self, ALC::Timestep t) override;

private:

	enum State {
		State_A,
		State_B
	};

	// states

	void BeginStateA(const State last, ALC::Entity self, ALC::Timestep t);
	void StateA(ALC::Entity self, ALC::Timestep t);

	void BeginStateB(const State last, ALC::Entity self, ALC::Timestep t);
	void StateB(ALC::Entity self, ALC::Timestep t);

	ALC::StateMachine<DemoChara, State, ALC::Entity, ALC::Timestep> m_state;
	float m_timer;
	float m_circleshootoffset;
	bool m_clockwise;
	float m_spinspeedmult;
};


#endif // !ALICE_CHARACTERS_DEMOCHARA_HPP