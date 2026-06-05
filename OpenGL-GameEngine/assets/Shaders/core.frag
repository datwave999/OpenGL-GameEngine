#version 460

out vec4 FragColour;

in vec4 colour;

void main(){
	FragColour = vec4(colour.xyz * 0.5 + vec3(0.5, 0.5, 0.5) , 1.0);
}