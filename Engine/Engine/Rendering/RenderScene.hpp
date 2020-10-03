#ifndef _RENDERER_RENDER_SCENE_HPP
#define _RENDERER_RENDER_SCENE_HPP
#include "ContentHandler.hpp"
#include "../General/Types.hpp"
#include "../General/Macros.hpp"
#include "Tools/RenderingLayer.hpp"

class Camera;

class RenderScene {
	PRIVATE_SINGLETON(RenderScene);

	vector<RenderingLayer*> renderingLayers;
	//vector<CanvasLayer*> canvasLayers;

	RenderScene();
	~RenderScene();

public:

	// events
	static void Init(const string& layersFile);
	static void Exit();
	static void Draw();

	// adding & removing
	static RenderingLayer* AddRenderer(IRenderer* renderer, const string& layer);
	static RenderingLayer* RemoveRenderer(IRenderer* renderer, const string& layer);
	static RenderingLayer* AddCamera(Camera* camera, const string& layer);
	static RenderingLayer* RemoveCamera(Camera* camera, const string& layer);
	static RenderingLayer* AddCanvasRenderer(ICanvasRenderer* renderer, const string& layer);
	static RenderingLayer* RemoveCanvasRenderer(ICanvasRenderer* renderer, const string& layer);

};

#endif // !_RENDERER_RENDER_SCENE_HPP