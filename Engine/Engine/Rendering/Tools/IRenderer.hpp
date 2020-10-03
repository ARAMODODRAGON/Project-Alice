#ifndef _RENDERING_RENDERER_HPP
#define _RENDERING_RENDERER_HPP
// the following classes are not actually interfaces but I was too lazy to change that

#include "../../General/Serialization.hpp"
#include "../../General/Math.hpp"
#include "../../General/Types.hpp"
#include "ContentTypes.hpp"

class Camera;

class IRenderer {

	Shader shader;
	string layername;

public:

	IRenderer();
	virtual ~IRenderer() = 0;

	// getters and setters
	void SetLayer(string layername = ""); // empty quotes will default
	string GetLayer() const { return layername; }
	Shader GetShader() const { return shader; }

	// pure virtual functions
	virtual void Draw() = 0;

protected:

	virtual void UpdateUniforms(Shader shader) = 0;

	RTTR_ENABLE()
};

class ICanvasRenderer {

	Shader shader;
	string layername;

public:

	ICanvasRenderer();
	virtual ~ICanvasRenderer() = 0;

	// getters and setters
	void SetLayer(string layername = ""); // empty quotes will default 
	string GetLayer() const { return layername; }
	Shader GetShader() const { return shader; }

	// pure virtual functions
	virtual void Draw() = 0;

protected:

	virtual void UpdateUniforms(Shader shader) = 0;

	RTTR_ENABLE()
};

#endif // !_RENDERING_RENDERER_HPP