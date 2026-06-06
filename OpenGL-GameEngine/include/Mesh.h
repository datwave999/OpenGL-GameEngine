#pragma once

#include"VertexArray.h"
#include"Buffer.h"
#include"Shader.h"

class Mesh
{
public:
	Mesh(GLfloat* vertices, GLsizeiptr vertexSize, GLuint* indices, GLsizeiptr indexSize);
	
	void RenderMesh();

	~Mesh();

private:
	Buffer* VBO;
	Buffer* EBO;
	VertexArray VAO;

	GLsizei indexCount;
};

