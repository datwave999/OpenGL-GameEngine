#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Mesh.h"

namespace GeometryUtils {

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

        // 1. Reset all vertex normals to zero. 
        // This is crucial in case the vertices had junk data or previous normals.
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

            // Compute the cross product (the perpendicular face normal).
            // NOTE: We deliberately do NOT normalize this vector yet!
            // The un-normalized cross product's magnitude is proportional to the triangle's area.
            // This means larger triangles will have more "weight" in the final smoothed normal.
            glm::vec3 faceNormal = glm::cross(edge1, edge2);

            // Add this face's normal to all three of its vertices
            v0.Normal += faceNormal;
            v1.Normal += faceNormal;
            v2.Normal += faceNormal;
        }

        // 3. Normalize the final accumulated normals
        // This averages out the directions of all the adjacent faces that shared this vertex.
        for (Vertex& vertex : vertices) {
            vertex.Normal = glm::normalize(vertex.Normal);
        }
    }

}