#version 450 core
layout (location = 0) in vec4 vertex;

layout (location = 0) out vec2 TexCoords;

uniform vec2 screenSize;

void main(){
	gl_Position = vec4(vertex.xy / screenSize, 0.0, 1.0);
	TexCoords = vertex.zw;
}