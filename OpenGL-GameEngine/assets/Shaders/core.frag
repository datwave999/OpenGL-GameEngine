#version 460

out vec4 FragTexture;

in vec2 TexCoords;

uniform sampler2D texture1;

void main(){
	FragTexture = texture(texture1, TexCoords);
}