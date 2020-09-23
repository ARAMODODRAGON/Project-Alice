#version 450 core
out vec4 _fragColor;

// recive from vert shader
in vec2 passUVCoord;

// other uniforms
uniform vec4 color;

layout (binding = 0) uniform sampler2D image;

void main() {

    vec4 texColor = texture(image, passUVCoord);

    // this checks if the color is completely trasparent
    if(texColor.a < 0.1) discard;

    // set frag color
    _fragColor = texColor * color;
}