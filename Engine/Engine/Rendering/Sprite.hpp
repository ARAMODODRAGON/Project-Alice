#ifndef _RENDERING_SPRITE_HPP
#define _RENDERING_SPRITE_HPP
#include "../Objects/Object.hpp"
#include "Tools/IRenderer.hpp"
#include "Rect.hpp"

class RenderScene;

class Sprite : public Component, public IRenderer {

	struct Vertex {
		vec2 position;
		vec2 uvCoord;
		Vertex() = default;
		Vertex(vec2 position_, vec2 uvCoord_)
			: position(position_), uvCoord(uvCoord_) { }
	};

	// texture, shader & uniforms
	Texture texture;
	unsigned int modelLoc, colorLoc;

	// vertex buffer object
	array<Vertex, 4> verticies;
	array<uvec3, 2> indicies;
	unsigned int VAO, VBO, EBO;

	// sprite properties
	vec2 pivot; // rotates and scales around this point
	vec2 scale;
	vec2 offset; // places the sprite at this offset from the object
	vec4 color;
	float rotation; // relative to object rotation

	// sprite tiling
	vec2 tilingSize;
	vec2 tilingOffset;
	vec2 tilingMargin;
	unsigned int tilingIndex;

	// private events
	void Start() override;
	void OnDestroy() override;

public:

	Sprite();
	~Sprite();

	// loading
	void LoadTexture(const string& textureName);

	// getters & setters
	vec2 GetPivot() const { return pivot; }
	void SetPivot(const vec2& pivot_) { pivot = pivot_; }

	vec2 GetScale() const { return scale; }
	void SetScale(const vec2& scale_) { scale = scale_; }

	vec2 GetOffset() const { return offset; }
	void SetOffset(const vec2& offset_) { offset = offset_; }

	float GetRotation() const { return rotation; }
	void SetRotation(const float& rotation_) { rotation = rotation_; }

	vec4 GetColor() const { return color; }
	void SetColor(const vec4& color_) { color = color_; }

	vec2 GetTilingSize() const { return tilingSize; }
	void SetTilingSize(const vec2& tilingSize_) { tilingSize = tilingSize_; UpdateVertexArray(); }

	vec2 GetTilingOffset() const { return tilingOffset; }
	void SetTilingOffset(const vec2& tilingOffset_) { tilingOffset = tilingOffset_; UpdateVertexArray(); }

	vec2 GetTilingMargin() const { return tilingMargin; }
	void SetTilingMargin(const vec2& tilingMargin_) { tilingMargin = tilingMargin_; UpdateVertexArray(); }

	unsigned int GetTilingIndex() const { return tilingIndex; }
	void SetTilingIndex(const unsigned int& tilingIndex_) { tilingIndex = tilingIndex_; UpdateVertexArray(); }

private:

	void UpdateVertexArray();
	void Draw() override;
	virtual void UpdateUniforms(Shader shader) override;

	RTTR_ENABLE(Component, IRenderer)
};

#endif // !_RENDERING_SPRITE_HPP