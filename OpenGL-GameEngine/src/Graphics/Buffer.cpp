#include "Buffer.h"

Buffer::Buffer(GLenum type, GLsizeiptr size, const void* data, GLenum drawType) : bufferType(type)
{
	glGenBuffers(1, &bufferID);
	glBindBuffer(bufferType, bufferID);
	glBufferData(bufferType, size, data, drawType);
}
 
void Buffer::bind() const {
	glBindBuffer(bufferType, bufferID);
}

void Buffer::unbind() const {
	glBindBuffer(bufferType, 0);
}

Buffer::~Buffer() {
	glDeleteBuffers(1, &bufferID);
}