#include "UIRenderer.hpp"
#include "RenderScene.hpp"

RTTR_REGISTRATION {
	registration::class_<UIRenderer>("UIRenderer")
		.public_object_constructor;
}

UIRenderer::UIRenderer() { }

UIRenderer::~UIRenderer() { }

void UIRenderer::Start() {
	// add
	RenderScene::AddCanvasRenderer(this);
}

void UIRenderer::OnDestroy() {
	// remove
	RenderScene::RemoveCanvasRenderer(this);
}

void UIRenderer::Draw(const vec2& screenSize) {

}
