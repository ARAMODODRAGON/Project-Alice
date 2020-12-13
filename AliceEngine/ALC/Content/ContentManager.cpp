#include "ContentManager.hpp"

namespace ALC {

	ContentStorage::ContentStorage() { }
	ContentStorage::~ContentStorage() {
		ContentManager::Clear(*this);
	}

	ContentStorage ContentManager::s_genericStorage;

	Texture ContentManager::LoadTexture(const string& path) {
		return LoadTexture(s_genericStorage, path);
	}

	Texture ContentManager::LoadTexture(ContentStorage& context, const string& path) {
		// check if it already exists
		auto it = context.m_textures.find(path);
		if (it != context.m_textures.end())
			return it->second;

		// load the texture and add it to the map
		Texture texture = Texture::Load(path);
		context.m_textures.emplace(path, texture);

		// return the newly loaded texture
		return texture;
	}

	Shader ContentManager::LoadShader(const string& path) {
		return LoadShader(s_genericStorage, path);
	}

	Shader ContentManager::LoadShader(ContentStorage& context, const string& path) {
		// check if it already exists
		auto it = context.m_shaders.find(path);
		if (it != context.m_shaders.end())
			return it->second;

		// load the shader and add it to the map
		Shader shader = Shader::Load(path);
		context.m_shaders.emplace(path, shader);

		// return the newly loaded shader
		return shader;
	}

	Shader ContentManager::LoadShaderSource(const string& source) {
		return LoadShaderSource(s_genericStorage, source);
	}

	Shader ContentManager::LoadShaderSource(ContentStorage& context, const string& source) {
		// check if it already exists
		auto it = context.m_shaders.find(source);
		if (it != context.m_shaders.end())
			return it->second;

		// load the shader and add it to the map
		Shader shader = Shader::LoadSource(source);
		context.m_shaders.emplace(source, shader);

		// return the newly loaded shader
		return shader;
	}

	void ContentManager::Clear() {
		ContentManager::Clear(s_genericStorage);
	}

	void ContentManager::Clear(ContentStorage& context) {

		// iterate through and delete all of the textures
		for (auto& [key, texture] : context.m_textures) {
			Texture::Delete(texture);
		}
		context.m_textures.clear();

		// iterate through and delete all of the shaders
		for (auto& [key, shader] : context.m_shaders) {
			Shader::Delete(shader);
		}
		context.m_shaders.clear();

	}

}