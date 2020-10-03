#include "RenderScene.hpp"
#include "../Core/Debugger.hpp"
#include "Camera.hpp"
#include <fstream>
#include <glew.h>

RenderScene::RenderScene() { }

RenderScene::~RenderScene() { }


void RenderScene::Init(const string& renderLayersFile) {

	// load file
	std::fstream file;
	file.open(renderLayersFile);
	if (!file.is_open()) throw std::runtime_error("could not open file to load layers!");
	json jfile;
	file >> jfile;
	file.close();

	#pragma region load the rendering layers

	if (jfile.contains("rendering layers") && jfile["rendering layers"].is_array()) {
		const json& layers = jfile["rendering layers"];
		for (size_t i = 0; i < layers.size(); i++) {
			RenderingLayer* renderinglayer = nullptr;

			try {
				renderinglayer = new RenderingLayer(layers[i]);
			} catch (std::exception& e) {
				DEBUG_ERROR("Failed to load " + VTOS(i) + ", exception: " + string(e.what()));
			} catch (...) {
				DEBUG_ERROR("Failed to load layer: " + VTOS(i) + ", unknown exception");
			}

			// add the layer
			if (renderinglayer != nullptr) {
				Get()->renderingLayers.push_back(renderinglayer);
			}

		}
	}

	#pragma endregion

	#pragma region load the canvas layers

	DEBUG_LOG("TODO: load canvas layers too");

	#pragma endregion

}

void RenderScene::Exit() {
	// delete rendering layers
	auto& renderingLayers = Get()->renderingLayers;
	for (RenderingLayer* rlayer : renderingLayers) {
		delete rlayer;
	}
	renderingLayers.clear();

	// delete the canvas layers
	// TODO

}

void RenderScene::Draw() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); 

	// first draw the rendering layers
	auto& renderingLayers = Get()->renderingLayers;
	for (RenderingLayer* rlayer : renderingLayers) {
		rlayer->Draw();
	}

	// next draw the canvas layers 
	// TODO

}

RenderingLayer* RenderScene::AddRenderer(IRenderer* renderer, const string& layer) {
	auto& renderingLayers = Get()->renderingLayers;
	if (renderingLayers.size() == 0) {
		DEBUG_ERROR("There are no rendering layers!");
		return nullptr;
	}
	// default to first layer
	if (layer == "") {
		renderingLayers[0]->AddRenderer(renderer);
		return renderingLayers[0];
	}

	// find layer and add renderer
	for (RenderingLayer* rlayer : renderingLayers) {
		if (rlayer->GetLayerName() == layer) {
			rlayer->AddRenderer(renderer);
			return rlayer; 
		}
	}

	DEBUG_ERROR("Could not add renderer to layer " + layer);
	return nullptr;
}

RenderingLayer* RenderScene::RemoveRenderer(IRenderer* renderer, const string& layer) {
	auto& renderingLayers = Get()->renderingLayers;
	if (renderingLayers.size() == 0) {
		DEBUG_ERROR("There are no rendering layers!");
		return nullptr;
	}
	// default to first layer
	if (layer == "") {
		renderingLayers[0]->RemoveRenderer(renderer);
		return renderingLayers[0];
	}

	// find layer and add renderer
	for (RenderingLayer* rlayer : renderingLayers) {
		if (rlayer->GetLayerName() == layer) {
			rlayer->RemoveRenderer(renderer);
			return rlayer;
		}
	}

	DEBUG_ERROR("Could remove renderer from layer " + layer);
	return nullptr;
}

RenderingLayer* RenderScene::AddCamera(Camera* camera, const string& layer) {
	auto& renderingLayers = Get()->renderingLayers;
	if (renderingLayers.size() == 0) {
		DEBUG_ERROR("There are no rendering layers!");
		return nullptr;
	}
	// default to first layer
	if (layer == "") {
		renderingLayers[0]->SetActiveCamera(camera);
		return renderingLayers[0];
	}

	// find layer and add camera
	for (RenderingLayer* rlayer : renderingLayers) {
		if (rlayer->GetLayerName() == layer) {
			rlayer->SetActiveCamera(camera);
			return rlayer;
		}
	}

	DEBUG_ERROR("Could not add camera to layer " + layer);
	return nullptr;
}

RenderingLayer* RenderScene::RemoveCamera(Camera* camera, const string& layer) {
	auto& renderingLayers = Get()->renderingLayers;
	if (renderingLayers.size() == 0) {
		DEBUG_ERROR("There are no rendering layers!");
		return nullptr;
	}
	// default to first layer
	if (layer == "") {
		renderingLayers[0]->UnsetActiveCamera(camera);
		return renderingLayers[0];
	}

	// find layer and remove camera
	for (RenderingLayer* rlayer : renderingLayers) {
		if (rlayer->GetLayerName() == layer) {
			rlayer->UnsetActiveCamera(camera);
			return rlayer;
		}
	}

	DEBUG_ERROR("Could not remove camera from layer " + layer);
	return nullptr;
}

RenderingLayer* RenderScene::AddCanvasRenderer(ICanvasRenderer* renderer, const string& layer) { 
	DEBUG_ERROR("Cant add canvas renderer");
	return nullptr;
}

RenderingLayer* RenderScene::RemoveCanvasRenderer(ICanvasRenderer* renderer, const string& layer) { 
	DEBUG_ERROR("Cant remove canvas renderer");
	return nullptr;
}
