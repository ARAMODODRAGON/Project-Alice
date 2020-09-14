#include "Sprite.hpp"
#include "../Core/Level.hpp"
#include <glew.h>

Sprite::Sprite()
	: offsetLoc(-1), rotationLoc(-1), layerLoc(-1)
	, VAO(-1), VBO(-1), EBO(-1)
	, pivot(0.0f)
	, scale(1.0f)
	, offset(0.0f)
	, rotation(0.0f)
	, layer(0.0f) {
	// add to render scene
	GetObject()->GetLevel()->GetRenderScene()->AddRenderer(this);

	// try to load a default shader
	LoadShader("Default");

	// initialize Vertex Array Object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// create buffers
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// load indicies
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uvec3) * 2, &indicies, GL_STATIC_DRAW);

	// load verticies
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, &verticies, GL_STREAM_DRAW);

	// set the verticies to location 0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	// set the uvCoords to location 1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)2);
	// set the verticies to location 2
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)4);

	// unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Sprite::~Sprite() {
	// remove from render scene
	GetObject()->GetLevel()->GetRenderScene()->RemoveRenderer(this);

	// delete buffers and vertex object
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void Sprite::LoadTexture(const string& textureName) {
	texture = GetObject()->GetLevel()->GetRenderScene()->GetContent()->LoadTexture(textureName);
}
void Sprite::LoadShader(const string& shaderName) {
	shader = GetObject()->GetLevel()->GetRenderScene()->GetContent()->LoadShader(shaderName);
	// TODO: load shader uniforms
}

Rect Sprite::GetTextureRect() const {
	Rect rect;
	rect.min = verticies[0].uvCoord; // bottom left
	rect.max = verticies[3].uvCoord; // top right
	return rect;
}
void Sprite::SetTextureRect(const Rect& rect) {
	verticies[0].uvCoord = rect.min; // bottom left
	verticies[1].uvCoord = vec2(rect.min.x, rect.max.y); // top left
	verticies[2].uvCoord = vec2(rect.max.x, rect.min.y); // bottom right
	verticies[3].uvCoord = rect.max; // top right
	UpdateVertexArray();
}
void Sprite::SetColor(const vec4& color_) {
	verticies[0].color = color_;
	verticies[1].color = color_;
	verticies[2].color = color_;
	verticies[3].color = color_;
	UpdateVertexArray();
}

void Sprite::UpdateVertexArray() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, &verticies, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Sprite::Draw(const Camera& camera) { 
	// TODO: draw code
}
