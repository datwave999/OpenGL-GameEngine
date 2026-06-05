#pragma once

#include <iostream>
#include <string>

#include <glad/glad.h>

class Texture
{
public:

	Texture(const std::string& path, const std::string& type);

	void Use(GLuint unitLocation = 0) const;
	std::string getType() { return type; }

	~Texture();

	// Prevents deep copies, only pass texture as pointer
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

private:
	GLuint textureID;
	std::string type;
	std::string path;
};

