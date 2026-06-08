#pragma once

#include<iostream>

#include<vector>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

class Buffer
{
public:
	Buffer() = default;
	Buffer(GLenum type, GLsizeiptr size, const void* data, GLenum drawType);

	void bind() const;
	void unbind() const;

	// Getters
	GLenum getBufferType() const { return bufferType; }

	~Buffer();

private:
	GLuint bufferID = 0;
	GLenum bufferType = GL_FALSE;
};

