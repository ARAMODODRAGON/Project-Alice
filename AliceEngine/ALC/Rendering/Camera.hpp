#ifndef ALC_RENDERING_CAMERA_HPP
#define ALC_RENDERING_CAMERA_HPP
#include "../General.hpp"

namespace ALC {

	class Camera {
	public:

		// default constructs the camera where the size matches the window size
		Camera();

		~Camera();

		// getters & setters

		vec2 GetPosition();
		void SetPosition(const vec2& position_);

		// camera size in world units
		vec2 GetCameraSize() const;
		// camera size in world units
		void SetCameraSize(const vec2& size_);

		// returns the transformation matrix
		mat4 GetTransform() const;

		// returns a matrix that converts from screen to world positions
		// uses the built in window size
		mat4 GetScreenToWorld() const;

		// returns a matrix that converts from screen to world positions
		mat4 GetScreenToWorld(const vec2& screensize) const;

		// returns a matrix that converts from world to screen positions
		// uses the built in window size
		mat4 GetWorldToScreen() const;

		// returns a matrix that converts from world to screen positions
		mat4 GetWorldToScreen(const vec2& screensize) const;

	private:
		vec2 m_position;
		vec2 m_size;
	};

}

#endif // !ALC_RENDERING_CAMERA_HPP