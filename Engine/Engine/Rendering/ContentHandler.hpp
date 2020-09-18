#ifndef _RENDERING_CONTENT_HANDLER_HPP
#define _RENDERING_CONTENT_HANDLER_HPP
#include "../General/Types.hpp"
#include "../General/Macros.hpp"
#include "ContentTypes.hpp"
#include "../Objects/FileIndex.hpp"

class ContentHandler {
	PRIVATE_SINGLETON(ContentHandler);

	// textures and shaders with file index
	using TexturePairType = pair<const string, Texture>;
	map<string, Texture> textures;
	using ShaderPairType = pair<const string, Shader>;
	map<string, Shader> shaders;
	FileIndex* textureIndex;
	FileIndex* shaderIndex;

	ContentHandler();
	~ContentHandler();

public:

	// events
	static void Init(const string& textureIndexPath_, const string& shaderIndexPath_);
	static void Clean();
	static void Exit();

	// functions
	static Texture LoadTexture(const string& textureName);
	static Shader LoadShader(const string& shaderName);

};

#endif // !_RENDERING_CONTENT_HANDLER_HPP