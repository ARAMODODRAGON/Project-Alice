#version 450 core
layout (location = 0) in vec2 TexCoords;

layout (location = 0) out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main(){
	float sampled = texture(text, TexCoords).r;
	color = vec4(textColor, 1.0) * sampled;
}