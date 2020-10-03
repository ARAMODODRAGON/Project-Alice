#include "IRenderer.hpp"
#include "../RenderScene.hpp"

RTTR_REGISTRATION {
	registration::class_<IRenderer>("IRenderer");
	registration::class_<ICanvasRenderer>("ICanvasRenderer");
}

IRenderer::IRenderer() {
	RenderingLayer* rlayer = RenderScene::AddRenderer(this, ""); // default
	if (rlayer) {
		shader = rlayer->GetShader();
		layername = rlayer->GetLayerName();
	}
}

IRenderer::~IRenderer() {
	if (layername != "") {
		RenderScene::RemoveRenderer(this, layername);
		shader = Shader(); // invalid shader
	}
}

void IRenderer::SetLayer(string layername) {
	if (layername != "") {
		RenderScene::RemoveRenderer(this, layername);
		shader = Shader(); // invalid shader
	}

	RenderingLayer* rlayer = RenderScene::AddRenderer(this, ""); // default
	if (rlayer) {
		shader = rlayer->GetShader();
		layername = rlayer->GetLayerName();
		UpdateUniforms(shader);
	}
}

ICanvasRenderer::ICanvasRenderer() { }

ICanvasRenderer::~ICanvasRenderer() { }

void ICanvasRenderer::SetLayer(string layername) {
	
}
