#include "OWPlayer.hpp"
#include <Engine/Common.hpp>

RTTR_REGISTRATION {
	registration::class_<OWPlayer>("OWPlayer")
		.public_object_constructor;
}

OWPlayer::OWPlayer() : sprite(nullptr), colorMix(0.0f), increaseMix(true) { }

OWPlayer::~OWPlayer() { }

void OWPlayer::Start() {
	DEBUG_LOG("My name is " + GetName() + " and I am at: " + VTOS(GetPosition()));

	// add components
	sprite = AddComponent<Sprite>();
	sprite->SetPivot(vec2(0.5f)); // center pivot
	sprite->SetColor(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	sprite->LoadTexture("Test 0");

	// create a seperate object to hold the camera
	Camera* cam = Make<Object>()->AddComponent<Camera>();
	vec2 size = GameContext::game->GetWindow()->GetScreenSize() / 60u;
	cam->SetCameraSize(size);
}

void OWPlayer::Update() {
	vec2 position = GetPosition();
	const static float delta = (1.0f / 60.0f);
	const static float move_delta = delta * 10.0f;
	const float slow_value = (Keyboard::GetKey(KeyCode::LeftShift) ? 0.4f : 1.0f);

	if (Keyboard::GetKey(KeyCode::ArrowUp))
		position.y += move_delta * slow_value;
	if (Keyboard::GetKey(KeyCode::ArrowDown))
		position.y -= move_delta * slow_value;
	if (Keyboard::GetKey(KeyCode::ArrowRight))
		position.x += move_delta * slow_value;
	if (Keyboard::GetKey(KeyCode::ArrowLeft))
		position.x -= move_delta * slow_value;

	SetPosition(position);

	const static float color_delta = delta * 0.33f;
	if (increaseMix) {
		colorMix += color_delta;
		if (colorMix > 1.0f) {
			colorMix = 1.0f;
			increaseMix = false;
		}
	} else {
		colorMix -= color_delta;
		if (colorMix < 0.0f) {
			colorMix = 0.0f;
			increaseMix = true;
		}
	}

	#define BRIGHT 0.1f
	const static vec4 primary_color = vec4(1.0f, BRIGHT, BRIGHT, 1.0f);
	const static vec4 secondary_color = vec4(BRIGHT, BRIGHT, 1.0f, 1.0f);
	sprite->SetColor(glm::mix(primary_color, secondary_color, colorMix));
}

