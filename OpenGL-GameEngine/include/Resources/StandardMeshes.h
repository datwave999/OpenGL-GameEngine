#pragma once

#include "Resources/AssetData.h"
#include <memory>

// typedef for the return type
using MeshData = std::pair<std::vector<Vertex>, std::vector<GLuint>>;

class StandardMeshes {
public:
    // flat shaded 1x1x1 cube
    static MeshData CreateCube(float uvScale = 1.0f);

    // smooth shaded sphere with adjustable resolution of diameter 1.0
    static MeshData CreateSphere(int sectors = 36, int stacks = 18);

    // flat plane of dimensions 100 x 1 x 100
    static MeshData CreatePlane(float uvScale = 1.0f);

    // overlay mesh
    static MeshData CreateOverlay();
};