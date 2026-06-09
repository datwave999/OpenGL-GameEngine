#version 460

out vec4 FragColour;

in vec2 TexCoords;
in vec3 Normal;

uniform sampler2D texture1;

void main(){
	FragColour = texture(texture1, TexCoords);
}