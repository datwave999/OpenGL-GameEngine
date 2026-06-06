#include "Mesh.h"

Mesh::Mesh(GLfloat* vertices, GLsizeiptr vertSize, GLuint* indices, GLsizeiptr indSize)
{
	VAO.bind();

	VBO = new Buffer(GL_ARRAY_BUFFER, vertSize, vertices, GL_STATIC_DRAW);
	EBO = new Buffer(GL_ELEMENT_ARRAY_BUFFER, indSize, indices, GL_STATIC_DRAW);

	VAO.addBuffer(*VBO, 0, 3, GL_FLOAT, 5 * sizeof(float), 0);
	VAO.addBuffer(*VBO, 1, 2, GL_FLOAT, 5 * sizeof(float), 3 * sizeof(float));

	indexCount = static_cast<GLsizei>(indSize / sizeof(GLuint));
}

void Mesh::RenderMesh()
{
	VAO.bind();
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

void Mesh::setupMesh()
{
}

Mesh::~Mesh()
{
	delete VBO;
	delete EBO;
}



