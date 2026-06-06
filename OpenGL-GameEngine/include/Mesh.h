#pragma once

#include"VertexArray.h"
#include"Buffer.h"
#include"Texture.h"
#include"Shader.h"

class Mesh
{
public:
	Mesh(GLfloat* vertices, GLsizeiptr vertexSize, GLuint* indices, GLsizeiptr indexSize, Texture* texture);
	
	void Render(Shader* shader, const glm::mat4& modelMatrix);

	~Mesh();

private:
	Buffer* VBO;
	Buffer* EBO;
	VertexArray VAO;

	Texture* texture;

	GLsizei indexCount;

	void setupMesh();
};

