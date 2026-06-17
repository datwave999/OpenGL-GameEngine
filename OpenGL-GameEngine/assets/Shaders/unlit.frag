#version 460 core
out vec4 FragColour;

in vec2 TexCoords;

uniform sampler2D materialDiffuse;

void main() {
    FragColour = texture(materialDiffuse, TexCoords);
}