#version 450 core
layout (location = 0) in vec4 vertex;

layout (location = 0) out vec2 TexCoords;

uniform vec2 screensize;

void main(){
	gl_Position = vec4(vertex.xy / screensize, 0.0, 1.0);
	TexCoords = vertex.zw;
}