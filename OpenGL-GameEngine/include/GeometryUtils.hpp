#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Mesh.h"

namespace GeometryUtils {
    // Only used for Meshes with unique vertices for each face
    inline void CalculateFlatNormals(std::vector<Vertex>& vertices, const std::vector<GLuint>& indices) {

        for (size_t i = 0; i < indices.size(); i += 3) {
            GLuint i0 = indices[i];
            GLuint i1 = indices[i + 1];
            GLuint i2 = indices[i + 2];

            Vertex& v0 = vertices[i0];
            Vertex& v1 = vertices[i1];
            Vertex& v2 = vertices[i2];

            glm::vec3 edge1 = v1.Position - v0.Position;
            glm::vec3 edge2 = v2.Position - v0.Position;

            glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));

            v0.Normal = faceNormal;
            v1.Normal = faceNormal;
            v2.Normal = faceNormal;
        }
    }

    inline void CalculateSmoothNormals(std::vector<Vertex>& vertices, const std::vector<GLuint>& indices) {

        // 1. Reset all vertex normals
        for (Vertex& vertex : vertices) {
            vertex.Normal = glm::vec3(0.0f);
        }

        // 2. Accumulate the face normals onto the shared vertices
        for (size_t i = 0; i < indices.size(); i += 3) {

            GLuint i0 = indices[i];
            GLuint i1 = indices[i + 1];
            GLuint i2 = indices[i + 2];

            Vertex& v0 = vertices[i0];
            Vertex& v1 = vertices[i1];
            Vertex& v2 = vertices[i2];

            glm::vec3 edge1 = v1.Position - v0.Position;
            glm::vec3 edge2 = v2.Position - v0.Position;

            // Get the face normal (with magnitude)
            glm::vec3 faceNormal = glm::cross(edge1, edge2);

            // Add this face's normal to all three of its vertices (will average this later by normalizing)
            v0.Normal += faceNormal;
            v1.Normal += faceNormal;
            v2.Normal += faceNormal;
        }

        // 3. Normalize the final accumulated normals
        for (Vertex& vertex : vertices) {
            vertex.Normal = glm::normalize(vertex.Normal);
        }
    }

}