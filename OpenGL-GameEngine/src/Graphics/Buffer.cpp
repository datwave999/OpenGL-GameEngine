#include "Graphics/Buffer.h"

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

	// Get GPU memory to write data to (GL_MAP_INVALIDATE_BUFFER_BIT = Orphaning)
    void* mappedMemory = glMapBufferRange(bufferType, offset, size,
        GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

    if (mappedMemory) {
		// Copy the data over 
        memcpy(mappedMemory, data, size);
        glUnmapBuffer(bufferType);
    }
}

Buffer::~Buffer() {
	glDeleteBuffers(1, &bufferID);
}