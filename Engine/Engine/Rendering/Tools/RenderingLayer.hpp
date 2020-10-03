#ifndef _RENDERINGTOOLS_RENDER_LAYER_HPP
#define _RENDERINGTOOLS_RENDER_LAYER_HPP
#include "IRenderer.hpp"

class RenderingLayer {

	// renderers that render on this layer
	list<IRenderer*> renderers;
	Shader objectShader; // shaders used on individual renderers
	unsigned int viewLoc, projLoc;

	// camera
	Camera* activeCamera;

	// framebuffer that everything renders to
	unsigned int frameBuffer;

	// output texture
	ivec2 size;
	unsigned int outputTexture;
	Shader postprocShader; // used when rendering the texture to the screen

	// quad that the texture renders to
	unsigned int VAO, VBO, EBO;

	// other properties
	string layername;
	vec4 clearcolor; // default {0, 0, 0, 0} (clear)

public:

	RenderingLayer(const json& data);
	~RenderingLayer();

	// events
	void Draw();

	// adders / removers
	void AddRenderer(IRenderer* renderer_);
	void RemoveRenderer(IRenderer* renderer_);
	void SetActiveCamera(Camera* activeCamera_);
	void UnsetActiveCamera(Camera* activeCamera_);

	// getters
	string GetLayerName() const { return layername; }
	Shader GetShader() const { return objectShader; }

};

#endif // !_RENDERINGTOOLS_RENDER_LAYER_HPP