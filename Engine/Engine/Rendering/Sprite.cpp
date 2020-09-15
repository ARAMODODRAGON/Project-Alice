#include "Sprite.hpp"
#include "../Core/Level.hpp"
#include <glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.hpp"

void Sprite::Start() {
	// add to render scene
	rendScene = GetObject()->GetLevel()->GetRenderScene();
	rendScene->AddRenderer(this);

	// try to load a default shader
	LoadShader("default");
}

void Sprite::OnDestroy() {
	// remove from render scene
	rendScene->RemoveRenderer(this);
}

Sprite::Sprite()
	: rendScene(nullptr)
	, viewLoc(-1), projLoc(-1), modelLoc(-1), colorLoc(-1)
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

	// default texture size
	drawSize = vec2(1.0f);

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
	texture = rendScene->GetContent()->LoadTexture(textureName);
	SetTextureRect(Rect(vec2(0.0f), texture.GetSize()));
	//SetTextureRect(Rect(vec2(0.0f), vec2(1.0f)));
}
void Sprite::LoadShader(const string& shaderName) {
	shader = rendScene->GetContent()->LoadShader(shaderName);

	// get uniforms
	viewLoc = glGetUniformLocation(shader, "viewMat");
	projLoc = glGetUniformLocation(shader, "projMat");
	modelLoc = glGetUniformLocation(shader, "modelMat");
	colorLoc = glGetUniformLocation(shader, "color");
}

Rect Sprite::GetTextureRect() const {
	Rect rect;
	rect.min = verticies[0].uvCoord * vec2(texture.GetSize()); // bottom left
	rect.max = verticies[3].uvCoord * vec2(texture.GetSize()); // top right
	return rect;
}
void Sprite::SetTextureRect(Rect rect) {
	// normalize the rect
	rect.min /= texture.GetSize();
	rect.max /= texture.GetSize();

	// set the coords
	verticies[0].uvCoord = rect.min;						// bottom left
	verticies[1].uvCoord = vec2(rect.min.x, rect.max.y);	// top left
	verticies[2].uvCoord = vec2(rect.max.x, rect.min.y);	// bottom right
	verticies[3].uvCoord = rect.max;						// top right
	drawSize = rect.max - rect.min;
	UpdateVertexArray();
}

void Sprite::UpdateVertexArray() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * 4, &verticies);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Sprite::Draw(const Camera& camera) {
	// quit
	if (!GetIsActive() || color.a == 0.0f) return;

	// bind vertex array, shader, and texture
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glUseProgram(shader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	// create the model matrix
	mat4 model;

	// pivot
	model = glm::translate(model, vec3(pivot * drawSize, 0.0f));

	// scale
	model = glm::scale(model, vec3(scale, 1.0f));

	// rotation
	model = glm::rotate(model, rotation, vec3(0.0f, 0.0f, 1.0f));

	// position
	vec3 position = vec3(GetObject()->GetPosition() + offset, layer);
	model = glm::translate(model, position);

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
