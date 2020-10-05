#include "RenderScene.hpp"
#include "../Core/Debugger.hpp"
#include "Camera.hpp"
#include "../Game.hpp"

RenderScene::RenderScene() { }

RenderScene::~RenderScene() { Clear(); }


void RenderScene::AddRenderer(IRenderer* renderer) {
	Get()->renderers.push_back(renderer);
}
void RenderScene::RemoveRenderer(IRenderer* renderer) {
	// get reference
	auto& renderers = Get()->renderers;

	for (auto it = renderers.begin(); it != renderers.end(); ++it) {
		if ((*it) == renderer) {
			renderers.erase(it);
			return;
		}
	}

	DEBUG_ERROR("Could not find the renderer");
}
void RenderScene::AddCamera(Camera* camera) {
	Get()->cameras.push_back(camera);
}
void RenderScene::RemoveCamera(Camera* camera) {
	// get reference
	auto& cameras = Get()->cameras;

	for (auto it = cameras.begin(); it != cameras.end(); ++it) {
		if ((*it) == camera) {
			cameras.erase(it);
			return;
		}
	}

	DEBUG_ERROR("Could not find the camera");
}
void RenderScene::AddCanvasRenderer(ICanvasRenderer* cavrenderer) {
	Get()->canvasRenderers.push_back(cavrenderer);
}
void RenderScene::RemoveCanvasRenderer(ICanvasRenderer* cavrenderer) {
	// get reference
	auto& canvasRenderers = Get()->canvasRenderers;

	for (auto it = canvasRenderers.begin(); it != canvasRenderers.end(); ++it) {
		if ((*it) == cavrenderer) {
			canvasRenderers.erase(it);
			return;
		}
	}

	DEBUG_ERROR("Could not find the canvas renderer");
}


void RenderScene::Init() { }

void RenderScene::Exit() { }

void RenderScene::Draw() {
	// get references
	auto& cameras = Get()->cameras;
	auto& renderers = Get()->renderers;
	auto& canvasRenderers = Get()->canvasRenderers;

	for (Camera* cam : cameras) {
		cam->UpdateView();
		for (IRenderer* rend : renderers) {
			rend->Draw(*cam); // call draw on all, let each one handle their own code
		}
	}

	vec2 windowSize = Game::Get()->GetWindow()->GetScreenSize();
	for (ICanvasRenderer* cvrend : canvasRenderers) {
		cvrend->Draw(windowSize);
	}

}

void RenderScene::Clear() {
	Get()->renderers.clear();
	Get()->cameras.clear();
}
