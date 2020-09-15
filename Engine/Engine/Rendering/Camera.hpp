#ifndef _RENDERING_CAMERA_HPP
#define _RENDERING_CAMERA_HPP
#include "../Objects/Object.hpp"
#include "Rect.hpp"
#include "RenderScene.hpp"

class RenderScene;

class Camera : public Component {

	// the renderscene that this is attached to
	RenderScene* rendScene;

	/// cameras properties
	vec2 size;
	mat4 view, proj;

	void Start() override;
	void OnDestroy() override;

public:

	Camera();
	~Camera();

	// event
	void UpdateView();

	// getters and setters
	vec2 GetCameraSize() const { return size; }
	const mat4& GetViewMat() const { return view; }
	const mat4& GetProjMat() const { return proj; }
	void SetCameraSize(const vec2& size_) { size = size_; }

	RTTR_ENABLE(Component)
};

#endif // !_RENDERING_CAMERA_HPP