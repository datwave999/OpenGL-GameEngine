#pragma once

#include<iostream>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"Buffer.h"

class VertexArray
{
public:
	VertexArray();

	void addBuffer(const Buffer& bufferObject, GLuint location, GLint size, GLenum type, GLsizei stride, int offset);

	void bind() const;
	void unbind() const;

	~VertexArray();

private:
	GLenum vaoID = 0;
};

