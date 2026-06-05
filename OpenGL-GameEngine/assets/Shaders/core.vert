#version 460

layout (location = 0) in vec3 pos;

out vec4 colour;

void main(){
	gl_Position = vec4(pos, 1.0);
	colour = gl_Position;
}