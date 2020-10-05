#ifndef _RENDERING_CONTENT_LOAD_FUNCTIONS_HPP
#define _RENDERING_CONTENT_LOAD_FUNCTIONS_HPP
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <glm/vec2.hpp>
using glm::uvec2;

#include "../Game.hpp"

unsigned int LoadTexture(const string& path, uvec2& outSize);
unsigned int LoadShader(const string& path);
unsigned int LoadShaderProgram(const vector<string>& shaderPaths);
FT_Face LoadFont(const string& path, const int width, const int height);

#endif // !_RENDERING_CONTENT_LOAD_FUNCTIONS_HPP