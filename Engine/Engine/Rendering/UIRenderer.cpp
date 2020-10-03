#include "UIRenderer.hpp"
#include "RenderScene.hpp"

RTTR_REGISTRATION {
	registration::class_<UIRenderer>("UIRenderer")
		.public_object_constructor;
}

UIRenderer::UIRenderer() { }

UIRenderer::~UIRenderer() { }

void UIRenderer::Start() { }

void UIRenderer::OnDestroy() { }

void UIRenderer::UpdateUniforms(Shader shader) { }

void UIRenderer::Draw() { }
