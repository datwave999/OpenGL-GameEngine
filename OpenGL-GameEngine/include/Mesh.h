#pragma once

#include"VertexArray.h"
#include"Buffer.h"
#include"Texture.h"

class Mesh
{
public:
	Mesh(GLfloat* vertices, GLsizeiptr vertexSize, GLuint* indices, GLsizeiptr indexSize, Texture* texture);
	
	void Render();

	~Mesh();

private:
	Buffer* VBO;
	Buffer* EBO;
	VertexArray VAO;

	Texture* texture;

	void setupMesh();
};

