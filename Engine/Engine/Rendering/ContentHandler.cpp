#include "ContentHandler.hpp"
#include <SDL_image.h>
#include <glew.h>
#include "ContentLoadFunctions.hpp"

ContentHandler::ContentHandler(const string& textureIndexPath_, const string& shaderIndexPath_)
	: textureIndex(textureIndexPath_), shaderIndex(shaderIndexPath_) { }

ContentHandler::~ContentHandler() {
	// delete textures
	for (TexturePairType& tp : textures) {
		GLuint id = tp.second;
		glDeleteTextures(1, &id);
	}
	textures.clear();

	// delete shaders
	for (ShaderPairType& sp : shaders)
		glDeleteProgram(sp.second);
	shaders.clear();
}

void ContentHandler::CleanUp() {
	// delete textures
	for (auto it = textures.begin(); it != textures.end(); ++it) {
		if (it->second.GetRefCount() == 1) {
			GLuint id = it->second;
			glDeleteTextures(1, &id);
			it = textures.erase(it);
		}
	}

	// delete shaders
	for (auto it = shaders.begin(); it != shaders.end(); ++it) {
		if (it->second.GetRefCount() == 1) {
			glDeleteProgram(it->second);
			it = shaders.erase(it);
		}
	}
}

Texture ContentHandler::LoadTexture(const string& textureName) {
	// first check if the texture is already loaded
	if (textures.find(textureName) != textures.end())
		return textures[textureName];

	// get the path
	string path = textureIndex.GetPath(textureName);

	// load the texture
	uvec2 size;
	GLuint textureID = ::LoadTexture(path, size);

	// check if it failed
	if (textureID == -1) return Texture();

	// create texture and return
	Texture texture(textureID, size);
	textures.insert(TexturePairType(textureName, texture));
	return texture;
}

Shader ContentHandler::LoadShader(const string& shaderName) {
	// check if the shader is already loaded
	if (shaders.find(shaderName) != shaders.end())
		return shaders[shaderName];

	GLuint shaderID = -1;

	// get all shader paths
	json j;
	shaderIndex.GetJSON(&j, shaderName);
	if (j.contains("shaders") && j["shaders"].is_array()) {
		vector<string> shaderPaths(3);
		for (size_t i = 0; i < j.size(); ++i) {
			if (j[i].is_string()) {
				shaderPaths.push_back(j[i].get<string>());
			}
		}

		shaderID = ::LoadShaderProgram(shaderPaths);
	}

	// check if it failed
	if (shaderID == -1) return Shader();

	// create shader and return
	Shader shader(shaderID);
	shaders.insert(ShaderPairType(shaderName, shader));
	return shader;
}
