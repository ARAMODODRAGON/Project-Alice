#type vertex
#version 450 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 uvCoord;

// matricies
uniform mat4 viewMat;
uniform mat4 projMat;
uniform mat4 modelMat;

// pass to the frag shader
out vec2 passUVCoord;

void main() {
    
    // set the pass variables
    passUVCoord = uvCoord;
    
    // set the position
    vec4 pos = vec4(position, 0.0, 1.0);

    // set to gl_Position
    gl_Position = projMat * viewMat * modelMat * pos;
}

#type fragment
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
   //_fragColor = vec4(1.0,0.0,0.0,1.0);
}