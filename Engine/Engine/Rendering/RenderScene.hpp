#ifndef _RENDERER_RENDER_SCENE_HPP
#define _RENDERER_RENDER_SCENE_HPP
#include "ContentHandler.hpp"
#include "../General/Types.hpp"
#include "../General/Macros.hpp"
#include "IRenderer.hpp"

class Camera;

class RenderScene {
	PRIVATE_SINGLETON(RenderScene);

	vector<IRenderer*> renderers;
	vector<Camera*> cameras;
	vector<ICanvasRenderer*> canvasRenderers;

public:

	RenderScene();
	~RenderScene();

	// functions
	static void AddRenderer(IRenderer* renderer);
	static void RemoveRenderer(IRenderer* renderer);
	static void AddCamera(Camera* camera);
	static void RemoveCamera(Camera* camera);
	static void AddCanvasRenderer(ICanvasRenderer* cavrenderer);
	static void RemoveCanvasRenderer(ICanvasRenderer* cavrenderer);

	// events
	static void Init();
	static void Exit();
	static void Draw();
	static void Clear();

};

#endif // !_RENDERER_RENDER_SCENE_HPP