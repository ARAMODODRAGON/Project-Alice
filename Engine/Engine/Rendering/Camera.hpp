#ifndef _RENDERING_CAMERA_HPP
#define _RENDERING_CAMERA_HPP
#include "../Objects/Object.hpp"
#include "RenderScene.hpp"

class Camera : public Component {

	/// cameras properties
	vec2 size;
	mat4 view, proj;

public:

	Camera();
	~Camera();

	// event
	void UpdateView();

	// getters and setters
	vec2 GetCameraSize() const { return size; }
	mat4 GetViewMat() const { return view; }
	mat4 GetProjMat() const { return proj; }
	void SetCameraSize(const vec2& size_) { size = size_; }

	RTTR_ENABLE(Component)
};

#endif // !_RENDERING_CAMERA_HPP