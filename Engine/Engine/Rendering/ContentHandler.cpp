#include "ContentHandler.hpp"
#include <SDL_image.h>
#include <glew.h>

ContentHandler::ContentHandler(const string& textureIndexPath_, const string& shaderIndexPath_)
	: textureIndex(textureIndexPath_), shaderIndex(shaderIndexPath_) { }

ContentHandler::~ContentHandler() { 
	// TODO: delete all textures/shaders
}

void ContentHandler::CleanUp() { 
	// TODO: delete all textures/shaders with only one reference
}

Texture ContentHandler::LoadTexture(const string& textureName) {
	return Texture();
}

Shader ContentHandler::LoadShader(const string& shaderName) {
	return Shader();
}
