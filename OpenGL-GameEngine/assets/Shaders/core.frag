#version 460

out vec4 FragColour;

in vec4 colour;
in vec2 texCoord;

uniform sampler2D texture1;

void main(){
	FragColour = texture(texture1, texCoord);
}