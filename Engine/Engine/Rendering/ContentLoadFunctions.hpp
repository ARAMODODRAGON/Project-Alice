#ifndef _RENDERING_CONTENT_LOAD_FUNCTIONS_HPP
#define _RENDERING_CONTENT_LOAD_FUNCTIONS_HPP
#include <string>
using std::string;
#include <vector>
using std::vector;

unsigned int LoadTexture(const string& path);
unsigned int LoadFragShader(const string& path);
unsigned int LoadVertShader(const string& path);
unsigned int LoadShaderProgram(const vector<string>& vertexShaderPaths, const vector<string>& fragmentShaderPaths);

#endif // !_RENDERING_CONTENT_LOAD_FUNCTIONS_HPP