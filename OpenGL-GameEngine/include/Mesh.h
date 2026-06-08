#pragma once

#include"VertexArray.h"
#include"Buffer.h"
#include"Shader.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh
{
public:
	Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);

	void RenderMesh();

	~Mesh();

private:
	std::unique_ptr<Buffer> VBO;
	std::unique_ptr<Buffer> EBO;
	VertexArray VAO;

	GLsizei indexCount;
};

