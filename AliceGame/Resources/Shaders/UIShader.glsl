#type vertex
#version 450 core
layout (location = 0) in vec4 vertex;

layout (location = 0) out vec2 TexCoords;

uniform vec2 screenSize;

void main(){
	gl_Position = vec4(vertex.xy / screenSize, 0.0, 1.0);
	TexCoords = vertex.zw;
}

#type fragment
#version 450 core
layout (location = 0) in vec2 TexCoords;

layout (location = 0) out vec4 color;

uniform sampler2D tex;
uniform vec3 texColor;
uniform int textureType;

void main(){
	if (textureType == 0){ // Render the font based on the R channel's value as the final fragment's alpha
		float sampled = texture(tex, TexCoords).r;
		color = vec4(texColor, 1.0) * sampled;
	} else if (textureType == 1){ // Render the texture as normal
		vec4 sampled = texture(tex, TexCoords);
		color = sampled;
	}
}