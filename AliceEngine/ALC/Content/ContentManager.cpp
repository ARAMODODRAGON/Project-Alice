#include "ContentManager.hpp"

namespace ALC {

	ContentStorage::ContentStorage() { }
	ContentStorage::~ContentStorage() {
		ContentManager::__RemoveContext(this);
		ContentManager::Clear(*this);
	}

	ContentStorage ContentManager::s_genericStorage;
	ContentStorage* ContentManager::s_contextStorage = nullptr;

	Texture ContentManager::LoadTexture(const string& path) {
		if (s_contextStorage)
			return LoadTexture(*s_contextStorage, path);
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
		if (s_contextStorage)
			return LoadShader(*s_contextStorage, path);
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
		if (s_contextStorage)
			return LoadShaderSource(*s_contextStorage, source);
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
		if (s_contextStorage)
			Clear(*s_contextStorage);
		Clear(s_genericStorage);
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

	void ContentManager::SetContext(ContentStorage& context) {
		s_contextStorage = &context;
	}

	void ContentManager::__RemoveContext(ContentStorage* context) {
		// remove the context only if it is the context
		if (s_contextStorage == context)
			s_contextStorage = nullptr;
	}

}