#ifndef _RENDERING_CONTENT_HANDLER_HPP
#define _RENDERING_CONTENT_HANDLER_HPP
#include "../General/Types.hpp"
#include "../General/Macros.hpp"
#include "ContentTypes.hpp"
#include "../Objects/FileIndex.hpp"

class ContentHandler {
	PRIVATE_SINGLETON(ContentHandler);

	using TexturePairType = pair<const string, Texture>;
	using ShaderPairType = pair<const string, Shader>;
	using FontPairType = pair<const string, Font>;

	// textures, shaders, and fonts with file index
	map<string, Texture> textures;
	map<string, Shader> shaders;
	map<string, Font> fonts;
	FileIndex* textureIndex;
	FileIndex* shaderIndex;
	FileIndex* fontIndex;

	ContentHandler();
	~ContentHandler();

public:

	// events
	static void Init(const string& textureIndexPath_, const string& shaderIndexPath_, const string& fontIndexPath_);
	static void Clean();
	static void Exit();

	// functions
	static Texture LoadTexture(const string& textureName);
	static Shader LoadShader(const string& shaderName);
	static Font LoadFont(const string& fontName);

};

#endif // !_RENDERING_CONTENT_HANDLER_HPP