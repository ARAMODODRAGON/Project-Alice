#include "ContentHandler.hpp"
#include <SDL_image.h>
#include <glew.h>
#include "ContentLoadFunctions.hpp"
#include "../Core/Debugger.hpp"

ContentHandler::ContentHandler()
	: textureIndex(nullptr), shaderIndex(nullptr) { }

ContentHandler::~ContentHandler() { 
	if (textureIndex) delete textureIndex;
	textureIndex = nullptr; 
	if (shaderIndex) delete shaderIndex;
	shaderIndex = nullptr;
}

void ContentHandler::Init(const string& textureIndexPath_, const string& shaderIndexPath_) {
	Get()->textureIndex = new FileIndex(textureIndexPath_);
	Get()->shaderIndex = new FileIndex(shaderIndexPath_);
}

void ContentHandler::Clean() {
	if (Get()->textureIndex == Get()->shaderIndex) {
		DEBUG_ERROR("ContentHandler has not been initialized");
		return;
	}

	auto& textures = Get()->textures;
	auto& shaders = Get()->shaders;
	auto& fonts = Get()->fonts;

	// delete textures
	for (auto it = textures.begin(); it != textures.end(); ++it) {
		if (it->second.GetRefCount() == 1) {
			GLuint id = it->second;
			glDeleteTextures(1, &id);
			it = textures.erase(it);
			// special case
			if (it == textures.end()) break;
		}
	}

	// delete shaders
	for (auto it = shaders.begin(); it != shaders.end(); ++it) {
		if (it->second.GetRefCount() == 1) {
			glDeleteProgram(it->second);
			it = shaders.erase(it);
		}
	}

	// delete fonts
	for (auto it = fonts.begin(); it != fonts.end(); ++it) {
		if (it->second.GetRefCount() == 1) {
			DEBUG_ERROR("Cant clean up fonts!");
			it = fonts.erase(it);
		}
	}
}

void ContentHandler::Exit() {
	if (Get()->textureIndex == Get()->shaderIndex) {
		DEBUG_ERROR("ContentHandler has not been initialized");
		return;
	}

	auto& textures = Get()->textures;
	auto& shaders = Get()->shaders;
	auto& fonts = Get()->fonts;

	// delete textures
	for (auto& tp : textures) {
		GLuint id = tp.second;
		glDeleteTextures(1, &id);
	}
	textures.clear();

	// delete shaders
	for (auto& sp : shaders)
		glDeleteProgram(sp.second);
	shaders.clear();

	// delete fonts
	for (auto& fp : fonts)
		DEBUG_ERROR("Cant delete font!");
	fonts.clear();

}

Texture ContentHandler::LoadTexture(const string& textureName) {
	if (Get()->textureIndex == Get()->shaderIndex) {
		DEBUG_ERROR("ContentHandler has not been initialized");
		return Texture();
	}
	auto& textures = Get()->textures;
	auto* textureIndex = Get()->textureIndex;

	// first check if the texture is already loaded
	if (textures.find(textureName) != textures.end())
		return textures[textureName];

	// get the path
	string path = textureIndex->GetPath(textureName);

	// load the texture
	uvec2 size;
	GLuint textureID = ::LoadTexture(path, size);

	// check if it failed
	if (textureID == -1) return Texture();

	// create texture and return
	Texture texture(textureID, size, textureName);
	textures.insert(TexturePairType(textureName, texture));
	return texture;
}

Shader ContentHandler::LoadShader(const string& shaderName) {
	if (Get()->textureIndex == Get()->shaderIndex) {
		DEBUG_ERROR("ContentHandler has not been initialized");
		return Shader();
	}
	auto& shaders = Get()->shaders;
	auto* shaderIndex = Get()->shaderIndex;

	// check if the shader is already loaded
	if (shaders.find(shaderName) != shaders.end())
		return shaders[shaderName];

	GLuint shaderID = -1;

	// get all shader paths
	json j;
	shaderIndex->GetJSON(&j, shaderName);
	if (j.contains("shaders")) {
		json& arr = j["shaders"];
		if (arr.is_array()) {

			vector<string> shaderPaths;
			shaderPaths.reserve(4);
			for (size_t i = 0; i < arr.size(); ++i) {
				if (arr[i].is_string()) {
					shaderPaths.push_back(arr[i].get<string>());
				}
			}

			shaderID = ::LoadShaderProgram(shaderPaths);
		}
	}

	// check if it failed
	if (shaderID == -1) return Shader();

	// create shader and return
	Shader shader(shaderID);
	shaders.insert(ShaderPairType(shaderName, shader));
	return shader;
}

Font ContentHandler::LoadFont(const string& fontName) {
	DEBUG_ERROR("Cannot load font!");
	return Font();
}
