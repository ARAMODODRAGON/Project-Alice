#ifndef ALICE_CHARACTERS_DEMOCHARA_HPP
#define ALICE_CHARACTERS_DEMOCHARA_HPP
#include <ALC\Entities\EntityCreationHandler.hpp>
#include <ALC\Physics\Rigidbody2D.hpp>
#include <ALC\Content\ContentManager.hpp>
#include <ALC\Rendering\SpriteBatch.hpp>
#include <ALC\Input\Keyboard.hpp>
#include <glm/gtx/vector_angle.hpp>

struct DemoBulletComponent final {
	float lifetime = 0.0f;
	float maxlifetime = 3.0f;
};

class DemoChara final : public ALC::Behavior {
	float timer;
	float circleshootoffset;
	bool clockwise;
	float spinspeedmult;
public:

	ALC::array<ALC::Texture, 22> textures;

	DemoChara();
	~DemoChara();

	void Start(ALC::Entity self) override;
	void OnDestroy(ALC::Entity self) override;

	void Shoot(ALC::Entity self, const float angle, const float speed, const ALC::vec2 position);
	
	void Update(ALC::Entity self, ALC::Timestep t) override;
	void LateUpdate(ALC::Entity self, ALC::Timestep t) override;

};


#endif // !ALICE_CHARACTERS_DEMOCHARA_HPP