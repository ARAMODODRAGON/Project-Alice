#include "Sprite.hpp"
#include "../Core/Level.hpp"
#include <glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.hpp"
#include "ContentHandler.hpp"

void Sprite::Start() {
	// add to render scene
	RenderScene::AddRenderer(this);

	// try to load a default shader
	LoadShader("default");
}

void Sprite::OnDestroy() {
	// remove from render scene
	RenderScene::RemoveRenderer(this);
}

Sprite::Sprite()
	: viewLoc(-1), projLoc(-1), modelLoc(-1), colorLoc(-1)
	, VAO(-1), VBO(-1), EBO(-1)
	, pivot(0.0f)
	, scale(1.0f)
	, offset(0.0f)
	, color(1.0f) // initialize to {1, 1, 1, 1} (white)
	, rotation(0.0f)
	, layer(0.0f) {

	// setup the vertex & indicies arrays
	verticies[0] = Vertex(vec2(0.0f, 0.0f), vec2(0.0f, 0.0f)); // bottom left
	verticies[1] = Vertex(vec2(0.0f, 1.0f), vec2(0.0f, 1.0f)); // top left
	verticies[2] = Vertex(vec2(1.0f, 0.0f), vec2(1.0f, 0.0f)); // bottom right
	verticies[3] = Vertex(vec2(1.0f, 1.0f), vec2(1.0f, 1.0f)); // top right
	indicies[0] = uvec3(0, 1, 2); // first tri
	indicies[1] = uvec3(3, 2, 1); // second tri

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
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, uvCoord));

	// unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Sprite::~Sprite() {
	// delete buffers and vertex object
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void Sprite::LoadTexture(const string& textureName) {
	texture = ContentHandler::LoadTexture(textureName);
	tilingSize = texture.GetSize();
	tilingOffset = vec2(0.0f);
	tilingMargin = vec2(0.0f);
	tilingIndex = 0;
	UpdateVertexArray();
}
void Sprite::LoadShader(const string& shaderName) {
	shader = ContentHandler::LoadShader(shaderName);

	// get uniforms
	viewLoc = glGetUniformLocation(shader, "viewMat");
	projLoc = glGetUniformLocation(shader, "projMat");
	modelLoc = glGetUniformLocation(shader, "modelMat");
	colorLoc = glGetUniformLocation(shader, "color");
	//DEBUG_LOG("View Location is: " + VTOS(viewLoc));
}

void Sprite::UpdateVertexArray() {
	// first calculate the new rect
	Rect rect;
	vec2 dist = tilingSize + tilingMargin; // dist between rects
	int columns = static_cast<int>((texture.GetSize().x - tilingOffset.x) / dist.x); // number of columns

	// special case where the texture doesnt load so it cant calculate any columns
	if (columns == 0) return;

	rect.min.x = (tilingIndex % columns) * dist.x + tilingOffset.x; // % returns remainder. remainder == column position
	rect.min.y = (tilingIndex / columns) * dist.y + tilingOffset.y; // / returns the row
	rect.max = rect.min + tilingSize;

	// normalize rect
	vec2 size = texture.GetSize();
	if (size.x != 0.0f && size.y != 0.0f) {
		rect.min /= size;
		rect.max /= size;
	} else {
		rect.min = vec2(0.0f);
		rect.max = vec2(1.0f);
	}

	// update the vertex array. the rect must be flipped vertically since the texture 0,0 is the top left
	verticies[0].uvCoord = vec2(rect.min.x, rect.max.y);	// bottom left
	verticies[1].uvCoord = rect.min;						// top left
	verticies[2].uvCoord = rect.max;						// bottom right
	verticies[3].uvCoord = vec2(rect.max.x, rect.min.y);	// top right

	// update the VAO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * 4, &verticies);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Sprite::Draw(const Camera& camera) {
	// quit
	if (!GetIsActive() || color.a == 0.0f) return;

	// create the model matrix
	mat4 model;
	// pivot
	model = glm::translate(model, -vec3(pivot / tilingSize, 0.0f));
	// scale
	model = glm::scale(model, vec3(scale * glm::normalize(tilingSize), 1.0f));
	// rotation
	model = glm::rotate(model, rotation, vec3(0.0f, 0.0f, 1.0f));
	// position
	vec3 position = vec3(GetObject()->GetPosition() + offset, layer);
	model = glm::translate(model, position);

	// bind vertex array, shader, and texture
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glUseProgram(shader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set uniforms
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.GetViewMat()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera.GetProjMat()));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform4f(colorLoc, color.r, color.g, color.b, color.a);

	// draw
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// unbind
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}
