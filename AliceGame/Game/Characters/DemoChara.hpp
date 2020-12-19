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

	DemoChara()
		: timer(0.0f), circleshootoffset(0.0f), clockwise(true), spinspeedmult(1.0f) {
		textures = {
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/1B Ninja.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/AAEEEIOU.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/AmazedFace.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Ayyad.jpg"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/ayyad.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Ayyyad.jpg"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Dwane_Face.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/HACK.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/IMG_2312.JPG"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/King_Sean.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/mohd.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/MrV.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/NIGHT.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/P.E.E.P.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/PizzaTime.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Random.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Random_Purple.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Random_Yellow.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Scott.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/SeanToxic.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Slick Shades Devil.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/unknown.png")
		};
	}
	~DemoChara() { }

	void Start(ALC::Entity self) override;
	void OnDestroy(ALC::Entity self) override;

	void Shoot(ALC::Entity self, const float angle, const float speed, const ALC::vec2 position);
	
	void Update(ALC::Entity self, ALC::Timestep t) override;
	void LateUpdate(ALC::Entity self, ALC::Timestep t) override;

};


#endif // !ALICE_CHARACTERS_DEMOCHARA_HPP