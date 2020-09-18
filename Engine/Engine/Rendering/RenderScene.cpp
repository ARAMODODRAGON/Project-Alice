#include "RenderScene.hpp"
#include "../Core/Debugger.hpp"
#include "Camera.hpp"

RenderScene::RenderScene() { }

RenderScene::~RenderScene() {
	renderers.clear();
	cameras.clear();
}


void RenderScene::AddRenderer(IRenderer* renderer) {
	renderers.push_back(renderer);
}

void RenderScene::RemoveRenderer(IRenderer* renderer) {
	for (auto it = renderers.begin(); it != renderers.end(); ++it) {
		if ((*it) == renderer) {
			renderers.erase(it);
			return;
		}
	}

	DEBUG_ERROR("Could not find the renderer");
}

void RenderScene::AddCamera(Camera* camera) {
	cameras.push_back(camera);
}

void RenderScene::RemoveCamera(Camera* camera) {
	for (auto it = cameras.begin(); it != cameras.end(); ++it) {
		if ((*it) == camera) {
			cameras.erase(it);
			return;
		}
	}

	DEBUG_ERROR("Could not find the camera");
}


void RenderScene::Draw() {
	for (Camera* cam : cameras) {
		cam->UpdateView();
		for (IRenderer* rend : renderers) {
			rend->Draw(*cam); // call draw on all, let each one handle their own code
		}
	}
}
