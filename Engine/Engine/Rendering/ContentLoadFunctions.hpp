#ifndef _RENDERING_CONTENT_LOAD_FUNCTIONS_HPP
#define _RENDERING_CONTENT_LOAD_FUNCTIONS_HPP

#include "../General/Types.hpp"
#include "../Game.hpp"

uint32 LoadTexture(const string& path, uvec2& outSize);
uint32 LoadShader(const string& path);
uint32 LoadShaderProgram(const vector<string>& shaderPaths);
FT_Face LoadFont(const string& path, const int width, const int height);

#endif // !_RENDERING_CONTENT_LOAD_FUNCTIONS_HPP