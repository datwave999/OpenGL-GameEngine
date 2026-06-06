#include "VertexArray.h"

VertexArray::VertexArray() {
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &vaoID);
}

void VertexArray::addBuffer(const Buffer& bufferObject, GLuint location, GLint size, GLenum type, GLsizei stride, int offset) {
	bind();
	bufferObject.bind();
	glVertexAttribPointer(location, size, type, GL_FALSE, stride, (void*)(intptr_t)offset);
	glEnableVertexAttribArray(location);
}

void VertexArray::bind() const {
	glBindVertexArray(vaoID);
}

void VertexArray::unbind() const {
	glBindVertexArray(0);
}