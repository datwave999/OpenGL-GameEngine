#include "StandardMeshes.h"
#include "GeometryUtils.hpp" 
#include <vector>

constexpr float PI = 3.14159265359f;

std::shared_ptr<Mesh> StandardMeshes::CreateCube() {
    std::vector<Vertex> vertices = {
        // FRONT FACE
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // 0
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, // 1
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}, // 2
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}, // 3

        // BACK FACE
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, // 4 
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // 5
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}, // 6
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}, // 7

        // LEFT FACE
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // 8
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, // 9
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}, // 10
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}, // 11

        // RIGHT FACE
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // 12
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, // 13
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}, // 14
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}, // 15

        // TOP FACE
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // 16
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, // 17
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}, // 18
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}, // 19

        // BOTTOM FACE
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // 20
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, // 21
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}, // 22
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}  // 23
    };

    std::vector<GLuint> indices = {
        0, 1, 2,  2, 3, 0,       // Front
        4, 5, 6,  6, 7, 4,       // Back
        8, 9, 10, 10, 11, 8,     // Left
        12, 13, 14, 14, 15, 12,  // Right
        16, 17, 18, 18, 19, 16,  // Top
        20, 21, 22, 22, 23, 20   // Bottom
    };

    GeometryUtils::CalculateFlatNormals(vertices, indices);

    return std::make_shared<Mesh>(vertices, indices);
}

std::shared_ptr<Mesh> StandardMeshes::CreateSphere(int sectors, int stacks) {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    float radius = 0.5f;

    for (int i = 0; i <= stacks; ++i) {
        float stackAngle = PI / 2 - i * PI / stacks;      // from pi/2 to -pi/2

        float y = radius * sinf(stackAngle);              // r * sin(u) -> Vertical height
        float xz = radius * cosf(stackAngle);             // r * cos(u) -> Radius of the slice

        for (int j = 0; j <= sectors; ++j) {
            float sectorAngle = j * 2 * PI / sectors;     // from 0 to 2pi

            float x = xz * cosf(sectorAngle);             // r * cos(u) * cos(v)
            float z = xz * sinf(sectorAngle);             // r * cos(u) * sin(v)

            float s = (float)j / sectors;
            float t = 1.0f - (float)i / stacks;

            Vertex vertex;
            vertex.Position = glm::vec3(x, y, z);
            vertex.TexCoords = glm::vec2(s, t);
            vertex.Normal = glm::vec3(0.0f);

            vertices.push_back(vertex);
        }
    }

    for (int i = 0; i < stacks; ++i) {
        int k1 = i * (sectors + 1);     // beginning of current stack
        int k2 = k1 + sectors + 1;      // beginning of next stack

        for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stacks - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    GeometryUtils::CalculateSmoothNormals(vertices, indices);

    return std::make_shared<Mesh>(vertices, indices);
}