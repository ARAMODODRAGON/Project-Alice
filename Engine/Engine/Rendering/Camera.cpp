#include "Camera.hpp"
#include "../Core/Level.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "../Game.hpp"

void Camera::Start() {
	RenderScene::AddCamera(this);
	SetCameraSize(Game::Get()->GetWindow()->GetScreenSize());

}
				 
void Camera::OnDestroy() {
	RenderScene::RemoveCamera(this);
}

Camera::Camera() { }
Camera::~Camera() { }

void Camera::UpdateView() {
	// update the view
	view = glm::translate(mat4(), -vec3(GetObject()->GetPosition(), 0.0f));
	//DEBUG_LOG("position is: " + VTOS(GetObject()->GetPosition()));

	// update the ortho
	vec2 halfsize = size * 0.5f;
	proj = glm::ortho(-halfsize.x, halfsize.x, -halfsize.y, halfsize.y, 10.0f, -10.0f);

}
