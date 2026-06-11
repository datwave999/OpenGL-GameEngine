#version 460

out vec4 FragColour;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture1;

void main(){
	

	// Final fragment color
	FragColour = texture(texture1, TexCoords);
}