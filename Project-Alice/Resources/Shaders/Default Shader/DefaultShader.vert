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