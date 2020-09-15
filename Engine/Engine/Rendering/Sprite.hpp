#ifndef _RENDERING_SPRITE_HPP
#define _RENDERING_SPRITE_HPP
#include "../Objects/Object.hpp"
#include "ContentTypes.hpp"
#include "IRenderer.hpp"
#include "Rect.hpp"

class RenderScene;

class Sprite : public Component, public IRenderer {

	// the renderscene that this is attached to
	RenderScene* rendScene;

	struct Vertex {
		vec2 position;
		vec2 uvCoord;
		Vertex() = default;
		Vertex(vec2 position_, vec2 uvCoord_)
			: position(position_), uvCoord(uvCoord_) { }
	};

	// texture, shader & uniforms
	Texture texture;
	vec2 drawSize; // the size of the area of the texture that is being drawn
	Shader shader;
	unsigned int viewLoc, projLoc, modelLoc;
	unsigned int colorLoc;

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
	float layer;

	void Start() override;
	void OnDestroy() override;

public:

	Sprite();
	~Sprite();

	// loading
	void LoadTexture(const string& textureName);
	void LoadShader(const string& shaderName);

	#pragma region Getters & Setters

	// getters & setters
	vec2 GetPivot() const { return pivot; }
	void SetPivot(const vec2& pivot_) { pivot = pivot_; }

	vec2 GetScale() const { return scale; }
	void SetScale(const vec2& scale_) { scale = scale_; }

	vec2 GetOffset() const { return offset; }
	void SetOffset(const vec2& offset_) { offset = offset_; }

	float GetRotation() const { return rotation; }
	void SetRotation(const float& rotation_) { rotation = rotation_; }

	int GetLayer() const { return int(layer); }
	void SetLayer(const int& layer_) { layer = float(layer_); }

	Rect GetTextureRect() const;
	void SetTextureRect(Rect rect);

	vec4 GetColor() const { return color; }
	void SetColor(const vec4& color_) { color = color_; }

	#pragma endregion

private:

	void UpdateVertexArray();
	void Draw(const Camera& camera) override;

	RTTR_ENABLE(Component, IRenderer)
};

#endif // !_RENDERING_SPRITE_HPP