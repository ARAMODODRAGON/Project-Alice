#type vertex
#version 450 core
layout (location = 0) in vec2 a_position;
layout (location = 1) in vec2 a_uvcoords;
layout (location = 2) in vec4 a_color;
layout (location = 3) in int a_textureIndex;

uniform mat4 u_transform;

out vec4 v_color;
out vec2 v_uvcoords;
out float v_textureIndex;

void main() {

	v_color = a_color;
	v_textureIndex = float(a_textureIndex);
	v_uvcoords = a_uvcoords;
	vec4 vertex = u_transform * vec4(a_position, 0.0, 1.0);
	gl_Position = vertex;
	
}

#type fragment
#version 450 core
out vec4 out_fragcolor;

const int u_TextureCount = INSERT_COUNT_HERE; // i insert a number where this is before loading the
uniform sampler2D u_textures[u_TextureCount];

in vec4 v_color;
in vec2 v_uvcoords;
in float v_textureIndex;

void main() {

	if (v_textureIndex == -1) {
		out_fragcolor = v_color;
	} else {
		out_fragcolor = texture(u_textures[int(v_textureIndex)], v_uvcoords) * v_color;
	} 
	
}
