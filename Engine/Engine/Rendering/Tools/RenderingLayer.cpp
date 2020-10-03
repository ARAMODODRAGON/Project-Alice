#include "RenderingLayer.hpp"
#include <glew.h>
#include "../../Game.hpp"
#include "../../Objects/JsonToObjectLoader.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "../Camera.hpp"

RenderingLayer::RenderingLayer(const json& data) 
	: viewLoc(-1), projLoc(-1), activeCamera(nullptr), layername(""), clearcolor(0.0f) {

	// get the window size
	size = ivec2(Game::Get()->GetWindow()->GetScreenSize());

	#pragma region load the json data

	// load layername
	if (data.contains("name") && data["name"].is_string())
		layername = data["name"].get<string>();

	// load shader
	if (data.contains("object shader") && data["object shader"].is_string())
		objectShader = ContentHandler::LoadShader(data["object shader"].get<string>());

	// load post shader
	if (data.contains("postproc shader") && data["postproc shader"].is_string())
		postprocShader = ContentHandler::LoadShader(data["postproc shader"].get<string>());

	// load clear color
	if (data.contains("clear color") && data["clear color"].is_object()) {
		variant v = clearcolor;
		JsonToObject(type::get<vec4>(), v, data["clear color"]);
		clearcolor = v.get_value<vec4>();
	}

	#pragma endregion

	#pragma region setup the framebuffer

	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	// setup the texture that it'll output to
	glGenTextures(1, &outputTexture);

	// bind the newly created texture
	glBindTexture(GL_TEXTURE_2D, outputTexture);

	// create an empty image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	// filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// set "outputTexture" as the color attachment #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, outputTexture, 0);

	// check
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw std::runtime_error("failed to create framebuffer!");

	#pragma endregion

	#pragma region now make the VAO to render with

	// verticies & indicies
	vec2 verticies[8] = {
		vec2(-1.0f, -1.0f), vec2(0.0f, 1.0f),	// bottom left
		vec2(-1.0f, 1.0f),	vec2(0.0f, 0.0f),	// top left
		vec2(1.0f, -1.0f),	vec2(1.0f, 0.0f),	// bottom right
		vec2(1.0f, 1.0f),	vec2(1.0f, 1.0f)	// top right
	};
	ivec3 indicies[2] = {
		ivec3(0, 1, 2),
		ivec3(2, 1, 3)
	};

	// gen VAO, VBO, and EBO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// load indicies
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ivec3) * 2, indicies, GL_STATIC_DRAW);

	// load verticies
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * 8, verticies, GL_STATIC_DRAW);

	// load positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec2) * 2, (GLvoid*)0);
	// load uvCoords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec2) * 2, (GLvoid*)sizeof(vec2));

	// unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	#pragma endregion

}

RenderingLayer::~RenderingLayer() {
	// delete everything
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteTextures(1, &outputTexture);
}

void RenderingLayer::Draw() {
	// dont bother since there's no camera
	if (activeCamera == nullptr) return;

	// check if the size of the framebuffer should change
	ivec2 windowsize = ivec2(Game::Get()->GetWindow()->GetScreenSize());
	if (windowsize != size) {
		// resize output texture
		glBindTexture(GL_TEXTURE_2D, outputTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glBindTexture(GL_TEXTURE_2D, 0); // unbind
	}

	// bind the frame buffer that we will render to
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	//glViewport(0, 0, size.x, size.y);
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearColor(clearcolor.x, clearcolor.y, clearcolor.z, clearcolor.w);
	glClear(GL_COLOR_BUFFER_BIT); 

	//// bind shader and uniforms
	//glUseProgram(objectShader);
	//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(activeCamera->GetViewMat()));
	//glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(activeCamera->GetProjMat()));
	//
	//// render all renderers
	//for (IRenderer* rend : renderers) {
	//	rend->Draw();
	//}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glViewport(0, 0, windowsize.x, windowsize.y);

	// now draw the framebuffer to screen
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// bind shader
	glUseProgram(postprocShader);

	// bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, outputTexture);

	// draw
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// unbind
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

}


void RenderingLayer::AddRenderer(IRenderer* renderer_) {
	renderers.push_back(renderer_);
}

void RenderingLayer::RemoveRenderer(IRenderer* renderer_) {
	renderers.remove(renderer_);
}

void RenderingLayer::SetActiveCamera(Camera* activeCamera_) { 
	// remove the prexisting camera
	if (activeCamera) {
		activeCamera->__RemoveLayer();
	}
	// set the new camera
	activeCamera = activeCamera_;
}

void RenderingLayer::UnsetActiveCamera(Camera* activeCamera_) { 
	if (activeCamera == activeCamera_) {
		activeCamera = nullptr;
	}
}
