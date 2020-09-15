#version 450 core
out vec4 _fragColor;

// recive from vert shader
in vec2 passUVCoord;

// other uniforms
uniform vec4 color;

layout (binding = 0) uniform sampler2D image;

void main() {

    _fragColor = texture(image, passUVCoord) * color;
    //_fragColor = color;
}