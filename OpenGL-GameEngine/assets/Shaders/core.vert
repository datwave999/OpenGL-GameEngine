#version 460

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

out vec4 colour;
out vec2 texCoord;

uniform mat4 model;

void main(){
	gl_Position = model * vec4(pos, 1.0);
	colour = gl_Position;
	texCoord = tex;
}