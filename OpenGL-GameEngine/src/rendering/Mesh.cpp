#include "Mesh.h"

Mesh::Mesh(GLfloat* vertices, GLsizeiptr vertSize, GLuint* indices, GLsizeiptr indSize, Texture* texture) : texture(texture)
{
	VAO.bind();

	VBO = new Buffer(GL_ARRAY_BUFFER, vertSize, vertices, GL_STATIC_DRAW);
	EBO = new Buffer(GL_ELEMENT_ARRAY_BUFFER, indSize, indices, GL_STATIC_DRAW);

	VAO.addBuffer(*VBO, 0, 3, GL_FLOAT, 5 * sizeof(float), 0);
	VAO.addBuffer(*VBO, 1, 2, GL_FLOAT, 5 * sizeof(float), 3 * sizeof(float));

	indexCount = static_cast<GLsizei>(indSize / sizeof(GLuint));
}

void Mesh::Render(Shader* shader, const glm::mat4& modelMatrix)
{
	VAO.bind();

	shader->setUniform("texture1", 0);
	shader->setUniform("model", modelMatrix);

	texture->Use(0);

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



