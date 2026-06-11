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

	// Update existing buffer data (for UBOs)
	void updateData(GLintptr offset, GLsizeiptr size, const void* data) const;

	// Getters
	GLuint getBufferID() const { return bufferID; }
	GLenum getBufferType() const { return bufferType; }

	~Buffer();

private:
	GLuint bufferID = 0;
	GLenum bufferType = GL_FALSE;
};

