#pragma once

#include"VertexArray.h"
#include"Buffer.h"

class Mesh
{
public:
	Mesh(GLfloat* vertices, GLsizeiptr vertexSize, GLuint* indices, GLsizeiptr indexSize);
	
	void Render();

	~Mesh();

private:
	Buffer* VBO;
	Buffer* EBO;
	VertexArray VAO;

	void setupMesh();
};

