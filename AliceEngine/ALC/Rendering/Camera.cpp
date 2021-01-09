#include "Camera.hpp"
#include <glm\gtc\matrix_transform.hpp>
#include "../SceneManager.hpp"

namespace ALC {

	Camera::Camera() : m_position(0.0f), m_size(0.0f) {
		m_size = SceneManager::GetWindow()->GetScreenSize();
	}

	Camera::~Camera() { }

	vec2 Camera::GetPosition() {
		return m_position;
	}

	void Camera::SetPosition(const vec2& position_) {
		m_position = position_;
	}

	vec2 Camera::GetCameraSize() const {
		return m_size;
	}

	void Camera::SetCameraSize(const vec2& size_) {
		m_size = size_;
	}

	mat4 Camera::GetTransform() const {
		mat4 transform(1.0f);
		// create the view
		transform = glm::translate(transform, -vec3(m_position, 0.0f));

		// create the ortho
		vec2 halfsize = m_size * 0.5f;
		transform = transform * glm::ortho(-halfsize.x, halfsize.x, -halfsize.y, halfsize.y);

		// return the transform
		return transform;
	}

	mat4 Camera::GetScreenToWorld() const {
		vec2 screensize = SceneManager::GetWindow()->GetScreenSize();
		mat4 screen = glm::ortho(0.0f, screensize.x, 0.0f, screensize.y);
		return glm::inverse(GetTransform()) * screen;
	}

	mat4 Camera::GetWorldToScreen() const {
		vec2 screensize = SceneManager::GetWindow()->GetScreenSize();
		mat4 screen = glm::ortho(0.0f, screensize.x, 0.0f, screensize.y);
		return glm::inverse(screen) * GetTransform();
	}

}
