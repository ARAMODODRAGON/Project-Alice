#version 450 core
out vec4 _fragColor;

in vec2 passUVCoord;
layout (binding = 0) uniform sampler2D image;

void main() {
	//_fragColor = texture(image, passUVCoord);
	_fragColor = vec4(1.0, 1.0, 1.0, 1.0);
}