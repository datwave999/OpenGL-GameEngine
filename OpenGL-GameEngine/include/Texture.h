#pragma once

#include <iostream>
#include <string>
#include <array>

#include <glad/glad.h>
#include<glm/glm.hpp>

class Texture
{
public:

	Texture(GLuint texID, const std::string& type = "texture_diffuse");
	Texture(const std::string& path, const std::string& type = "texture_diffuse");
	Texture(glm::vec4 color, const std::string& type = "texture_diffuse");

	void Use(GLuint textureUnit = 0) const;

	GLuint getTextureID() { return textureID; }
	std::string getType() { return type; }
	std::string getPath() { return path; }

	~Texture();

	// Missing Texture (Magenta-Black 2x2 Grid)
	static unsigned int CreateDefaultTexture();

	// Prevents deep copies, only pass texture as pointer
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

private:
	GLuint textureID;
	std::string type;
	std::string path;
};

