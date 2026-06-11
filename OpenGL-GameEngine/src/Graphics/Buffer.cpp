#include "Buffer.h"

Buffer::Buffer(GLenum type, GLsizeiptr size, const void* data, GLenum drawType) : bufferType(type)
{
	glGenBuffers(1, &bufferID);
	glBindBuffer(bufferType, bufferID);
	glBufferData(bufferType, size, data, drawType);
}

void Buffer::bindBase(GLuint index) const{
	glBindBufferBase(bufferType, index, bufferID);
}
 
void Buffer::bind() const {
	glBindBuffer(bufferType, bufferID);
}

void Buffer::unbind() const {
	glBindBuffer(bufferType, 0);
}

void Buffer::updateData(GLintptr offset, GLsizeiptr size, const void* data) const
{
	glBindBuffer(bufferType, bufferID);

	// Orphaning method so that CPU isn't stalled
	glBufferData(bufferType, size, nullptr, GL_DYNAMIC_DRAW);

	glBufferSubData(bufferType, offset, size, data);
}

Buffer::~Buffer() {
	glDeleteBuffers(1, &bufferID);
}