#include "deprecated/Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices)
{
	VAO.bind();

	VBO = std::make_unique<Buffer>(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	EBO = std::make_unique<Buffer>(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	// Position (Location 0) - 3 floats
	VAO.addVertexBuffer(*VBO, 0, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, Position));
	// Normal (Location 1) - 3 floats
	VAO.addVertexBuffer(*VBO, 1, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, Normal));
	// Texture Coordinates (Location 2) - 2 floats
	VAO.addVertexBuffer(*VBO, 2, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, TexCoords));

	// Index buffer
	VAO.setIndexBuffer(*EBO);

	indexCount = static_cast<GLsizei>(indices.size());
}

void Mesh::RenderMesh()
{
	VAO.bind();
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

Mesh::~Mesh()
{
}



