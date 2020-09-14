#ifndef _RENDERER_RENDER_SCENE_HPP
#define _RENDERER_RENDER_SCENE_HPP
#include "ContentHandler.hpp"
#include "../General/Types.hpp"
#include "IRenderer.hpp"

class Camera;

class RenderScene {

	ContentHandler* content;

	vector<IRenderer*> renderers;
	vector<Camera*> cameras;

public:

	RenderScene(ContentHandler* content_);
	~RenderScene();

	// functions
	void AddRenderer(IRenderer* renderer);
	void RemoveRenderer(IRenderer* renderer);
	void AddCamera(Camera* camera);
	void RemoveCamera(Camera* camera);

	// getter
	ContentHandler* GetContent() const { return content; }

	// events
	void Draw();

};

#endif // !_RENDERER_RENDER_SCENE_HPP