#include "Texture.h"

#include <STB/stb_image.h>	

Texture::Texture(const std::string& path, const std::string& type) : path(path), type(type), textureID(0)
{

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // OpenGL expects 0.0 on Y axis to be bottom
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	if (data) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cerr << "Failed to load texture: " << path << std::endl;
	}

	stbi_image_free(data);
}

void Texture::Use(GLuint textureUnit) const {
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

Texture::~Texture() {
	glDeleteTextures(1, &textureID);
}
