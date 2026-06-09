#include "VertexArray.h"
#include <cassert>

VertexArray::VertexArray() {
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &vaoID);
}

void VertexArray::addVertexBuffer(const Buffer& VBO, GLuint location, GLint size, GLenum type, GLsizei stride, int offset) {
	assert(VBO.getBufferType() == GL_ARRAY_BUFFER && "Fatal: Passed a non-VBO to addVertexBuffer!");

	bind();
	VBO.bind();
	glVertexAttribPointer(location, size, type, GL_FALSE, stride, (void*)(intptr_t)offset);
	glEnableVertexAttribArray(location);
}

void VertexArray::setIndexBuffer(const Buffer& EBO)
{
	assert(EBO.getBufferType() == GL_ELEMENT_ARRAY_BUFFER && "Fatal: Passed a non-EBO to setIndexBuffer!");

	bind();
	EBO.bind();
}



void VertexArray::bind() const {
	glBindVertexArray(vaoID);
}

void VertexArray::unbind() const {
	glBindVertexArray(0);
}