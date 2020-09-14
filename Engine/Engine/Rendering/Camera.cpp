#include "Camera.hpp"
#include "../Core/Level.hpp"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() {
	GetObject()->GetLevel()->GetRenderScene()->AddCamera(this);
}

Camera::~Camera() {
	GetObject()->GetLevel()->GetRenderScene()->RemoveCamera(this);
}

void Camera::UpdateView() {
	// update the view
	view = glm::translate(mat4(), vec3(GetObject()->GetPosition(), 0.0f));

	// update the ortho
	vec2 halfsize = size * 0.5f;
	proj = glm::ortho(-halfsize.x, halfsize.x, -halfsize.y, halfsize.y);
}
